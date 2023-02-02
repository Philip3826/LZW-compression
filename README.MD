# LZW Compression
## Структура на проекта

### File Structure

Всеки файл или директория имат следните общи характеристики записани в архива.

    1.Дължина на името - std::size_t nameLength 
    2.Име във формата на път (в структурата на архива) - name
    3.Булева стойност дали файла е директория - isDir

В зависимост дали файла е директория или не записваме следните различни данни.

Ако е директория записваме:

    isEmpty - булева променлива казваща ни дали току-що прочетената папка е празна.

Ако е файл записваме:

    1. Стойност показваща колко добре е компресиран файла - uint16_t compressedPercentage
    2. Стойност служеща за идентификатор дали коректно е записано съдържанието на файла - std::size_t checkSum
    3. Дължина на компресираните данни - std::size_t contentLength
    4. Компресираните данни записани под формата на uint16_t числа

Тази сттруктура ми позволява линейно (спрямо размера на архива) да намирам нужните ми файлове.

## Classes of the project

### Compressor

Класът Compressor се грижи за компресията и декомпресията на данните , също и за изчисляването на checkSum който се слага като последен елемент на вектора от компресирани данни който функцията compress връща.

    class Compressor 
    {
    public:
        std::vector<uint16_t> compress(std::string fileContents);
        std::string decompress(std::vector<uint16_t> compressedContent);
    private:
        void initializeDictionary(std::unordered_map<std::string,uint16_t>& dictionary);
        void initializeDecompressDictionary(std::unordered_map<uint16_t, std::string>& dictionary);
    };

### LzwFile

Докато работя с компресиран файл било то за четене или писане бива пазен в следния клас:

    class LzwFile {
	std::string readFileContents(std::filesystem::path filePath);
	std::filesystem::path filePath;
	std::vector<uint16_t> contents;
	uint16_t compressedPercentage;
	std::size_t checkSum{ 0 };
    public:
	    LzwFile(std::filesystem::path filePath);
	    LzwFile(std::filesystem::path filePath, uint16_t perc, std::vector<uint16_t> compressedContents, std::size_t checkSum);
        ~LzwFile() = default;
	    LzwFile(const LzwFile& other);
        LzwFile& operator=(const LzwFile& other);
        std::filesystem::path getFilePath() const;
        void setFilePath(std::filesystem::path filePath);
        std::vector<uint16_t> getContents() const;
        uint16_t getPercentage() const;
        std::size_t getCheckSum() const;
        bool correctCheckSum();
        Compressor comp;
    };

Когато бива извикан конструктора:
    
    LzwFile(std::filesystem::path filePath);

Данните на файла биват компресирани от comp и след това се чете последната стойност на вектора върнат от comp и се запазва в checkSum.

### ArchiveHandler

Работата с писане и четене на файлове и директории установява класът ArchiveHandler.

    class ArchiveHandler {

    public:
        void createArchive(std::filesystem::path newArchivePath, std::vector<std::filesystem::path> files);
        void extractArchive(std::filesystem::path destination, std::filesystem::path archive);
        void unzipFile(std::filesystem::path pathInArchive, std::filesystem::path archivePath, std::filesystem::path  destination);
        void printInfo(std::filesystem::path archivePath);
        void checkForCorruption(std::filesystem::path archivePath);
        void refreshFile(std::filesystem::path newFile, std::filesystem::path pathInArchive, std::filesystem::path archivePath);
    private:
        void compressDirectory(std::filesystem::path root,std::filesystem::path directoryPath,std::ofstream& archive);
        void writeFileBlock(LzwFile file, std::ofstream& archive);
        LzwFile readFileBlock(std::filesystem::path filelPath ,std::ifstream& archive);
        void unzipDirectory(std::filesystem::path destination,std::filesystem::path pathInFolder,std::ifstream& archive,std::size_t archiveSize);
        std::size_t findFile(std::filesystem::path filePath, std::ifstream& archive);
        std::size_t findEndOfDir(std::string dirName, std::filesystem::path archivePath,std::ifstream& archive);
        void skipFileBlock(std::ifstream& archive,std::size_t pos);
    };

Когато се компресират или декомпресират директории се прави рекурсивно по дълбочината на директория , тоест ако се намери папка в папката която компресираме отново се вика функцията за компресация на директория над новата папка преди да завършим компресацията на останалите файлове в родителската папка. За кракто описание какво правят функциите имат doxygen-style коментар над всяка от дефинициите им.

### ActionHandler

ActionHandler се грижи за викане на правилната функция от ArchiveHandler и за това потребителя да е въвел правилните команди в противен случай се връща грешка.

## Main

В мейн функцията приемаме от командия ред комаандата на потребителя и след като тя бъде валидирана от ActionHandler се извиква ActionHandler.executeCommand(). Ако има някаква грешка с командите се изписва каква е тя и се принтира съобщение за как се използва програмата.

## Installation

Изтеглете архивираната програма и я разархивирайте в папка по ваше желание. Отворере папката с Visual Studio и билднете проекта. След това в command prompt се насочете до папката на проекта и отидета на ./out/build . Въведета командите
    
    cmake ../..
    start "LZW Compression.sln"
    msbuild "LZW Compression.sln"

След това може да използвате програмата чрез

    ./Debug/app.exe command [command arguments]
