#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <limits>
#include <filesystem> // Для получения списка файлов

/// <summary>
/// Получить список файлов в текущей директории, начинающихся на "animation" и с расширением ".txt"
/// </summary>
/// <returns>Вектор имён файлов</returns>
std::vector<std::string> getAnimationFiles() {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();

            // Проверка начала имени файла и расширения
            if (filename.rfind("animation", 0) == 0 && entry.path().extension() == ".txt") {
                files.push_back(filename);
            }
        }
    }
    return files;
}

/// <summary>
/// Получить содержимое файла
/// </summary>
/// <param name="filename">Имя файла</param>
/// <returns>Содержимое файла</returns>
std::string getFileContent(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Ошибка открытия файла: " << filename << "\n";
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());
    file.close();
    return content;
}

/// <summary>
/// Разделить строку по разделителю
/// </summary>
/// <param name="content">Исходная строка</param>
/// <param name="delimiter">Разделитель</param>
/// <returns>Вектор подстрок</returns>
std::vector<std::string> split(const std::string& content, const std::string& delimiter) {
    std::vector<std::string> parts;
    size_t start = 0;
    size_t end = content.find(delimiter);
    while (end != std::string::npos) {
        parts.push_back(content.substr(start, end - start));
        start = end + delimiter.length();
        end = content.find(delimiter, start);
    }
    parts.push_back(content.substr(start));
    return parts;
}

/// <summary>
/// Проиграть анимацию, выводя кадры с задержкой
/// </summary>
/// <param name="frames">Вектор кадров</param>
void playAnimation(const std::vector<std::string>& frames) {
    while (true) {
        for (const auto& frame : frames) {
            // Очистка экрана
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
            std::cout << frame << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(33));
        }
    }
}

/// <summary>
/// Получить корректный выбор пользователя
/// </summary>
/// <param name="max_option">Максимальный номер опции</param>
/// <returns>Номер выбранной опции</returns>
int getUserChoice(int max_option) {
    int choice;
    while (true) {
        std::cout << "Введите номер: ";
        if (std::cin >> choice) {
            if (choice >= 1 && choice <= max_option) {
                return choice;
            }
        }
        std::cout << "Некорректный ввод, попробуйте снова.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    std::vector<std::string> files = getAnimationFiles();

    if (files.empty()) {
        std::cout << "Файлы анимаций не найдены.\n";
        return 0;
    }

    while (true) {
        // Отобразить меню
        std::cout << "\nВыберите анимацию:\n";
        for (int i = 0; i < files.size(); ++i) {
            std::cout << i + 1 << ". " << files[i] << "\n";
        }
        std::cout << files.size() + 1 << ". Выход\n";

        int choice = getUserChoice(files.size() + 1);

        if (choice == files.size() + 1) {
            // Выход
            break;
        } else {
            // Проигрывание выбранной анимации
            std::string filename = files[choice - 1];
            std::vector<std::string> content = split(getFileContent(filename), "FRAME");
            playAnimation(content);
        }
    }

    return 0;
}