def main():
    file_with_names = 'Files.txt'
    output_file = 'newMain.cpp'
    # Открываем файл Files.txt и читаем имена файлов
    with open(file_with_names, 'r') as file_list:
        files = [line.strip() for line in file_list.readlines()]

    # Открываем файл newMain.cpp для записи
    with open(output_file, 'w') as new_file:
        # Проходим по всем файлам из списка
        for file_name in files:
            try:
                # Открываем каждый файл и читаем его содержимое
                with open(file_name, 'r') as f:
                    lines = f.readlines()
                    new_file.write(f'\n\n\n//{file_name}\n')
                
                # Копируем строки, которые не начинаются с #include"
                for line in lines:
                    if not line.startswith('#include"') and not line.startswith('#include "') and not line.startswith('#pragma once'):
                        new_file.write(line)
            except FileNotFoundError:
                print(f"Файл {file_name} не найден.")
            except Exception as e:
                print(f"Ошибка при обработке файла {file_name}: {e}")

if __name__ == "__main__":
    main()
