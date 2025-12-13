#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <csignal>
#include <ctime>
#include <map>
#include <algorithm>  // Добавлено для std::replace

// Флаги для управления циклом
volatile bool running = true;
volatile bool shouldStop = false;

// Карта соответствия нот MIDI
std::map<std::string, std::string> noteMapping = {
    {"24","C1"},{"25","C#1"},{"26","D1"},{"27","D#1"},{"28","E1"},{"29","F1"},{"2A","F#1"},{"2B","G1"},{"2C","G#1"},{"2D","A1"},{"2E","A#1"},{"2F","B1"},
    {"30","C2"},{"31","C#2"},{"32","D2"},{"33","D#2"},{"34","E2"},{"35","F2"},{"36","F#2"},{"37","G2"},{"38","G#2"},{"39","A2"},{"3A","A#2"},{"3B","B2"},
    {"3C","C3"},{"3D","C#3"},{"3E","D3"},{"3F","D#3"},{"40","E3"},{"41","F3"},{"42","F#3"},{"43","G3"},{"44","G#3"},{"45","A3"},{"46","A#3"},{"47","B3"},
    {"48","C4"},{"49","C#4"},{"4A","D4"},{"4B","D#4"},{"4C","E4"},{"4D","F4"},{"4E","F#4"},{"4F","G4"},{"50","G#4"},{"51","A4"},{"52","A#4"},{"53","B4"},
    {"54","C5"},{"55","C#5"},{"56","D5"},{"57","D#5"},{"58","E5"},{"59","F5"},{"5A","F#5"},{"5B","G5"},{"5C","G#5"},{"5D","A5"},{"5E","A#5"},{"5F","B5"},
    {"60","C6"}
};

// Обработчик сигнала для корректного завершения
void signalHandler(int signum) {
    (void)signum; // Используем параметр, чтобы избежать предупреждения
    std::cout << "\nПолучен сигнал завершения. Завершаем работу..." << std::endl;
    running = false;
    shouldStop = true;
}

// Функция для получения текущего времени в формате строки
std::string getCurrentTime() {
    time_t now = time(0);
    char buffer[80];
    struct tm* timeinfo = localtime(&now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return std::string(buffer);
}

// Функция для получения имени файла без запрещенных символов
std::string getSafeFilename() {
    std::string timestamp = getCurrentTime();
    std::string filename = "midi_log_" + timestamp + ".txt";
    
    // Заменяем недопустимые символы в имени файла
    std::replace(filename.begin(), filename.end(), ':', '-');
    std::replace(filename.begin(), filename.end(), ' ', '_');
    
    return filename;
}

// Функция для записи данных в файл
void logToFile(std::ofstream& logFile, const std::string& type, const std::string& hexNote, 
               const std::string& noteName = "", int velocity = -1, int program = -1) {
    
    std::string timestamp = getCurrentTime();
    std::string logEntry = "[" + timestamp + "] ";
    
    if (type == "NOTE_ON") {
        logEntry += "NOTE_ON: Hex=" + hexNote + " Note=" + noteName + " Velocity=" + std::to_string(velocity);
    } else if (type == "NOTE_OFF") {
        logEntry += "NOTE_OFF: Hex=" + hexNote + " Note=" + noteName;
    } else if (type == "PROGRAM_CHANGE") {
        logEntry += "PROGRAM_CHANGE: Program=" + std::to_string(program);
    } else if (type == "STATUS") {
        logEntry += "STATUS: " + hexNote;
    } else if (type == "CONTROL") {
        logEntry += "CONTROL: " + hexNote;
    } else if (type == "PITCH_BEND") {
        logEntry += "PITCH_BEND: " + hexNote;
    }
    
    // Записываем в файл
    logFile << logEntry << std::endl;
    logFile.flush(); // Сразу записываем на диск
    
    // Выводим в консоль
    std::cout << logEntry << std::endl;
}

// Основная функция чтения MIDI данных
void readMidiData(const std::string& devicePath) {
    std::string filename = getSafeFilename();
    std::ofstream logFile(filename);
    
    if (!logFile.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filename << std::endl;
        return;
    }
    
    std::cout << "Логирование запущено. Данные записываются в файл: " << filename << std::endl;
    std::cout << "Для остановки нажмите Ctrl+C" << std::endl;
    
    logToFile(logFile, "STATUS", "Запуск логгирования MIDI данных");
    logToFile(logFile, "STATUS", "Устройство: " + devicePath);
    
    int fd = open(devicePath.c_str(), O_RDWR | O_NOCTTY);
    if (fd < 0) {
        logToFile(logFile, "STATUS", "Ошибка: Не удалось открыть устройство " + devicePath);
        std::cerr << "Ошибка: Не удалось открыть устройство " << devicePath << std::endl;
        std::cerr << "Ошибка системы: " << strerror(errno) << std::endl;
        logFile.close();
        return;
    }
    
    logToFile(logFile, "STATUS", "Устройство успешно открыто");
    
    // Настройка последовательного порта
    struct termios tty;
    tcgetattr(fd, &tty);
    cfsetospeed(&tty, B115200);
    cfsetispeed(&tty, B115200);
    tty.c_cflag |= (CLOCAL | CREAD | CS8);
    tty.c_cc[VMIN] = 1;
    tcsetattr(fd, TCSANOW, &tty);
    
    logToFile(logFile, "STATUS", "Настройка порта завершена, скорость: 115200 бод");
    
    unsigned char byte;
    int programNumber = -1; // Номер текущей программы (инструмента)
    
    while (running) {
        ssize_t bytesRead = read(fd, &byte, 1);
        
        if (bytesRead <= 0) {
            if (shouldStop) break;
            if (bytesRead < 0) {
                // Ошибка чтения
                std::cerr << "Ошибка чтения: " << strerror(errno) << std::endl;
                break;
            }
            continue;
        }
        
        // Пропускаем системные сообщения
        if (byte == 0xF8 || byte == 0xFE) {
            continue;
        }
        
        // Проверяем статус байт
        if (byte & 0x80) {
            // NOTE ON (0x90) - нажатие клавиши
            if ((byte & 0xF0) == 0x90) {
                unsigned char note, velocity;
                if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                    char hexStr[3];
                    snprintf(hexStr, sizeof(hexStr), "%02X", note);
                    std::string hexNote = hexStr;
                    
                    std::string noteName = "Unknown";
                    if (noteMapping.find(hexNote) != noteMapping.end()) {
                        noteName = noteMapping[hexNote];
                    }
                    
                    if (velocity == 0) {
                        // velocity=0 интерпретируем как NOTE OFF
                        logToFile(logFile, "NOTE_OFF", hexNote, noteName);
                    } else {
                        logToFile(logFile, "NOTE_ON", hexNote, noteName, velocity, programNumber);
                    }
                }
            }
            // NOTE OFF (0x80) - отпускание клавиши
            else if ((byte & 0xF0) == 0x80) {
                unsigned char note, velocity;
                if (read(fd, &note, 1) > 0 && read(fd, &velocity, 1) > 0) {
                    char hexStr[3];
                    snprintf(hexStr, sizeof(hexStr), "%02X", note);
                    std::string hexNote = hexStr;
                    
                    std::string noteName = "Unknown";
                    if (noteMapping.find(hexNote) != noteMapping.end()) {
                        noteName = noteMapping[hexNote];
                    }
                    
                    logToFile(logFile, "NOTE_OFF", hexNote, noteName);
                }
            }
            // PROGRAM CHANGE (0xC0) - смена программы/инструмента
            else if ((byte & 0xF0) == 0xC0) {
                unsigned char program;
                if (read(fd, &program, 1) > 0) {
                    programNumber = program;
                    logToFile(logFile, "PROGRAM_CHANGE", "", "", -1, programNumber);
                }
            }
            // CONTROL CHANGE (0xB0) - управляющие изменения
            else if ((byte & 0xF0) == 0xB0) {
                unsigned char controller, value;
                if (read(fd, &controller, 1) > 0 && read(fd, &value, 1) > 0) {
                    char hexController[3];
                    snprintf(hexController, sizeof(hexController), "%02X", controller);
                    
                    // Логируем только важные контроллеры
                    if (controller == 0x07) { // Volume
                        logToFile(logFile, "CONTROL", 
                                 "Controller=0x" + std::string(hexController) + " (Volume) Value=" + std::to_string(value));
                    } else if (controller == 0x0A) { // Pan
                        logToFile(logFile, "CONTROL",
                                 "Controller=0x" + std::string(hexController) + " (Pan) Value=" + std::to_string(value));
                    } else {
                        logToFile(logFile, "CONTROL",
                                 "Controller=0x" + std::string(hexController) + " Value=" + std::to_string(value));
                    }
                }
            }
            // PITCH BEND (0xE0) - изгиб тона
            else if ((byte & 0xF0) == 0xE0) {
                unsigned char lsb, msb;
                if (read(fd, &lsb, 1) > 0 && read(fd, &msb, 1) > 0) {
                    int bendValue = ((msb << 7) | lsb) - 8192; // Центр = 0
                    logToFile(logFile, "PITCH_BEND",
                             "Value=" + std::to_string(bendValue));
                }
            }
            // Другие MIDI сообщения
            else {
                char hexByte[3];
                snprintf(hexByte, sizeof(hexByte), "%02X", byte);
                logToFile(logFile, "STATUS", 
                         "Неизвестное MIDI сообщение: 0x" + std::string(hexByte));
            }
        } else {
            // Данные без статусного байта (продолжение предыдущего сообщения)
            char hexByte[3];
            snprintf(hexByte, sizeof(hexByte), "%02X", byte);
            logToFile(logFile, "STATUS", 
                     "Данные без статуса: 0x" + std::string(hexByte));
        }
    }
    
    close(fd);
    logToFile(logFile, "STATUS", "Логгирование остановлено");
    logFile.close();
    
    std::cout << "\nЛоггирование завершено. Данные сохранены в файле: " << filename << std::endl;
}

// Функция для автоматического поиска MIDI-устройств
std::string findMidiDevice() {
    const char* possibleDevices[] = {
        "/dev/dmmidi1",
        "/dev/dmmidi2",
        "/dev/ttyACM0",
        "/dev/ttyACM1",
        "/dev/ttyUSB0",
        "/dev/ttyUSB1",
        "/dev/midi1",
        "/dev/midi2",
        "/dev/midi",
        "/dev/ttyS0",
        "/dev/ttyS1",
        "/dev/ttyS2",
        "/dev/ttyS3"
    };
    
    int numDevices = sizeof(possibleDevices) / sizeof(possibleDevices[0]);
    
    std::cout << "Поиск MIDI устройств..." << std::endl;
    
    for (int i = 0; i < numDevices; i++) {
        if (access(possibleDevices[i], F_OK) != -1) {
            std::cout << "✓ Найдено устройство: " << possibleDevices[i] << std::endl;
            return std::string(possibleDevices[i]);
        } else {
            std::cout << "  Проверено: " << possibleDevices[i] << " (не найдено)" << std::endl;
        }
    }
    
    return "";
}

int main() {
    // Устанавливаем обработчики сигналов
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    
    std::cout << "=== Yamaha MIDI Data Logger ===" << std::endl;
    std::cout << "Программа для записи MIDI данных с синтезатора Yamaha" << std::endl;
    std::cout << "===================================" << std::endl;
    
    // Автоматический поиск устройства
    std::string devicePath = findMidiDevice();
    
    if (devicePath.empty()) {
        std::cout << "\nMIDI устройство не найдено автоматически." << std::endl;
        std::cout << "Введите путь к устройству вручную: ";
        std::getline(std::cin, devicePath);
        
        if (devicePath.empty()) {
            devicePath = "/dev/dmmidi1"; // Значение по умолчанию
            std::cout << "Используется устройство по умолчанию: " << devicePath << std::endl;
        }
    }
    
    std::cout << "\nНастройки логгирования:" << std::endl;
    std::cout << "1. Записывать все MIDI сообщения" << std::endl;
    std::cout << "2. Записывать только ноты (Note On/Off)" << std::endl;
    std::cout << "3. Записывать ноты и программы" << std::endl;
    std::cout << "\nВыберите режим (1-3, по умолчанию 3): ";
    
    std::string modeInput;
    std::getline(std::cin, modeInput);
    
    int mode = 3; // Режим по умолчанию
    if (!modeInput.empty()) {
        try {
            mode = std::stoi(modeInput);
            if (mode < 1 || mode > 3) mode = 3;
        } catch (...) {
            mode = 3;
        }
    }
    
    std::cout << "\nНачинаю логгирование с устройства: " << devicePath << std::endl;
    std::cout << "Режим: " << mode << std::endl;
    std::cout << "===================================" << std::endl;
    std::cout << "Для остановки нажмите Ctrl+C" << std::endl;
    std::cout << "===================================" << std::endl;
    
    // Запускаем чтение данных
    readMidiData(devicePath);
    
    std::cout << "\nПрограмма завершена." << std::endl;
    return 0;
}