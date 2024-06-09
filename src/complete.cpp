#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <cctype>
#include <sstream>

using namespace std;

void banner()
{
    cout << "\n";
    cout << "████████╗ █████╗ ███╗   ███╗███████╗███╗   ██╗████████╗\n";
    cout << "╚══██╔══╝██╔══██╗████╗ ████║██╔════╝████╗  ██║╚══██╔══╝\n";
    cout << "   ██║   ███████║██╔████╔██║█████╗  ██╔██╗ ██║   ██║   \n";
    cout << "   ██║   ██╔══██║██║╚██╔╝██║██╔══╝  ██║╚██╗██║   ██║   \n";
    cout << "   ██║   ██║  ██║██║ ╚═╝ ██║███████╗██║ ╚████║   ██║   \n";
    cout << "   ╚═╝   ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝╚═╝  ╚═══╝   ╚═╝   \n";
    cout << "\n";
}

enum Status
{
    TODO,
    DONE
};

enum Kategori
{
    WORK,
    PERSONAL,
    OTHER
};

struct Task
{
    string nama;
    string deskripsi;
    Status status;
    Kategori kategori;
    vector<Task *> subtasks;
    vector<Task *> dependency;
    string tanggal;

    Task(string n, string d, Status s, Kategori k, string t) : nama(n), deskripsi(d), status(s), kategori(k), tanggal(t) {}
};

vector<Task *> TodoTasks;
vector<Task *> CompletedTasks;

void handleInvalidInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\x1b[31mInvalid Input\x1b[0m" << endl;
}

string statusToString(Status s)
{
    switch (s)
    {
    case TODO:
        return "TODO";
    case DONE:
        return "DONE";
    default:
        return "UNKNOWN";
    }
}

string kategoriToString(Kategori k)
{
    switch (k)
    {
    case WORK:
        return "WORK";
    case PERSONAL:
        return "PERSONAL";
    case OTHER:
        return "OTHER";
    default:
        return "UNKNOWN";
    }
}

string getTanggalHariIni()
{
    time_t t = time(0);
    tm *now = localtime(&t);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", now);
    return string(buffer);
}

Kategori inputKategori()
{
    while (true)
    {
        int pilihan;
        cout << "Pilih Kategori (0: WORK, 1: PERSONAL, 2: OTHER): ";
        cin >> pilihan;
        if(cin.fail() || pilihan < 0 || pilihan > 2){
            handleInvalidInput();
        }
        else if(pilihan == 0){
            return WORK;
        }
        else if(pilihan == 1){
            return PERSONAL;
        }
        else if(pilihan == 2){
            return OTHER;
        }
    }
}



bool containsLetters(const string &str)
{
    for (char c : str)
    {
        if (isalpha(c))
        {
            return true;
        }
    }
    return false;
}

void displayTasks(const vector<Task *> &tasks, int level=0)
{
    string indent(level * 4, ' ');
    if(level==0){
        cout << "\n";
        cout << left << setw(5) << "No"
            << left << setw(20) << "Nama"
            << left << setw(30) << "Deskripsi"
            << left << setw(10) << "Status"
            << left << setw(15) << "Kategori"
            << left << setw(15) << "Tanggal"
            << left << setw(20) << "Dependencies" << endl;
        cout << string(105, '=') << endl;
    }

    int index = 1;
    for (const Task *task : tasks)
    {
        cout << indent << left << setw(5) << index++
             << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori)
             << left << setw(15) << task->tanggal;

        if (!task->dependency.empty())
        {
            cout << left << setw(20);
            for (const Task *dep : task->dependency)
            {
                cout << dep->nama << " ";
            }
        }
        cout << endl;

        if (!task->subtasks.empty())
        {
            displayTasks(task->subtasks, level + 1);
        }
    }
}

void displayMainTasks(const vector<Task *> &tasks)
{
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }

    cout << "\n";
    cout << left << setw(5) << "No"
         << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(10) << "Status"
         << left << setw(15) << "Kategori"
         << left << setw(15) << "Tanggal"
         << left << setw(20) << "Dependencies" << endl;
    cout << string(105, '=') << endl;

    int index = 1;
    for (const Task *task : tasks)
    {
        cout << left << setw(5) << index++
             << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori)
             << left << setw(15) << task->tanggal;

        if (!task->dependency.empty())
        {
            cout << left << setw(20);
            for (const Task *dep : task->dependency)
            {
                cout << dep->nama << " ";
            }
        }
        cout << endl;
    }
}

void displayAllTasks(const vector<Task *> &tasks)
{
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayTasks(tasks);
}

vector<Task *> createDependency(const vector<Task *> &tasksList)
{
    vector<Task *> dependencies;
    if (tasksList.empty())
    {
        return dependencies;
    }

    char choice;
    do
    {
        cout << "Apakah task ini memiliki dependency? (Y/N): ";
        cin >> choice;
        cin.ignore();

        if (choice == 'y' || choice == 'Y')
        {
            displayMainTasks(tasksList);
            string dependencyInput;
            cout << "Pilih nomor mana saja yang menjadi dependency (pisahkan dengan spasi): ";
            getline(cin, dependencyInput);

            if (!containsLetters(dependencyInput))
            {
                stringstream ss(dependencyInput);
                int dependencyTask;
                while (ss >> dependencyTask)
                {
                    if (dependencyTask >= 1 && dependencyTask <= tasksList.size())
                    {
                        dependencies.push_back(tasksList[dependencyTask - 1]);
                        return dependencies;
                        break;
                    }
                    else
                    {
                        cout << "Nomor task " << dependencyTask << " tidak valid. Dependency ini akan diabaikan." << endl;
                        break;
                    }
                }
            }
            else
            {
                handleInvalidInput();
            }
        }
        else if (choice == 'n' || choice == 'N')
        {
            break;
        }
        else
        {
            handleInvalidInput();
        }
    } while (true);

    return dependencies;
}


Task *createTask()
{
    string nama, deskripsi, tanggal;

    cout << "\n========  MEMBUAT TASK  ========\n";
    cout << "Masukkan Nama Task: ";
    getline(cin, nama);
    cout << "Masukkan Deskripsi Task: ";
    getline(cin, deskripsi);

    tanggal = getTanggalHariIni();
    Status status = TODO;
    Kategori kategori = inputKategori();

    // Clear input state
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Task *newTask = new Task(nama, deskripsi, status, kategori, tanggal);
    newTask->dependency = createDependency(TodoTasks);

    TodoTasks.push_back(newTask);
    cout << "Task berhasil dibuat!" << endl;
    return newTask;
}


Task *createSubtask()
{
    string nama, deskripsi, tanggal;

    cout << "Masukkan Nama subtask: ";
    getline(cin, nama);

    cout << "Masukkan Deskripsi subtask: ";
    getline(cin, deskripsi);

    tanggal = getTanggalHariIni();
    Status status = TODO;
    Kategori kategori = WORK;

    Task *newTask = new Task(nama, deskripsi, status, kategori, tanggal);

    return newTask;
}

void displayTasksByCategory(const vector<Task *> &tasks)
{
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    Kategori category = inputKategori();
    vector<Task *> filteredTasks;
    for (const Task *task : tasks)
    {
        if (task->kategori == category)
        {
            filteredTasks.push_back(const_cast<Task *>(task));
        }
    }
    displayTasks(filteredTasks);
}

void addSubtask(vector<Task *> &tasks)
{
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayMainTasks(tasks);

    cout << "Pilih nomor task yang ingin ditambahkan subtask: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > tasks.size())
    {
        handleInvalidInput();
        return;
    }

    Task *task = tasks[nomorTask - 1];
    Task *newSubtask = createSubtask();

    // Set kategori subtask sesuai dengan parent
    newSubtask->kategori = task->kategori;

    task->subtasks.push_back(newSubtask);

    cout << "Subtask berhasil ditambahkan ke task '" << task->nama << "'." << endl;
}

void markAllSubtaskDone(Task *task) {
    task->status = DONE;

    // Set semua subtasknya ke done
    for (Task *subtask : task->subtasks) {
        markAllSubtaskDone(subtask);
    }
}

void markFirstQueueAsDone(vector<Task *> &TodoTasks, vector<Task *> &completedTasks) {
    if(TodoTasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    Task *task = TodoTasks.front(); 
    markAllSubtaskDone(task);
    TodoTasks.erase(TodoTasks.begin());

    completedTasks.push_back(task);
}

bool canBeMarkedAsDone(Task *task) {
    for (Task *dep : task->dependency) {
        if (dep->status != DONE) {
            return false;
        }
    }
    return true;
}

void markAsDone(vector<Task *> &tasks, vector<Task *> &completedTasks) {
    if (tasks.empty()) {
        cout << "Tidak ada task yang tersedia untuk ditandai sebagai selesai." << endl;
        return;
    }

    displayTasks(tasks);

    cout << "Pilih nomor task yang ingin ditandai sebagai selesai: ";
    int taskNumber;
    cin >> taskNumber;

    if (cin.fail() || taskNumber < 1 || taskNumber > tasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *task = tasks[taskNumber - 1];

    if (!canBeMarkedAsDone(task)) {
        cout << "Task tidak dapat ditandai sebagai selesai karena ada dependensi yang belum selesai." << endl;
        return;
    }

    markAllSubtaskDone(task);

    tasks.erase(tasks.begin() + (taskNumber - 1));
    completedTasks.push_back(task);

    cout << "Task dan subtasks berhasil ditandai sebagai selesai." << endl;
}


void deleteTask(vector<Task *> &tasks)
{
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayAllTasks(tasks);

    cout << "Pilih nomor task yang ingin dihapus: ";
    int nomorTask;
    cin >> nomorTask;

    if (cin.fail() || nomorTask < 1 || nomorTask > tasks.size())
    {
        handleInvalidInput();
        return;
    }

    delete tasks[nomorTask - 1];
    tasks.erase(tasks.begin() + (nomorTask - 1));

    cout << "Task berhasil dihapus." << endl;
}

void deleteLastTask(vector<Task *> &tasks)
{
    if (tasks.empty())
    {
        cout << "Tidak ada Todo task yang bisa dihapus." << endl;
        return;
    }

    tasks.pop_back();

    cout << "Task Terakhir berhasil dihapus." << endl;
}

void displayStack()
{
    if (TodoTasks.empty())
    {
        cout << "Stack kosong." << endl;
        return;
    }

    cout << "\n========  STACK  ========\n";
    cout << left << setw(5) << "No"
         << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(10) << "Status"
         << left << setw(15) << "Kategori"
         << left << setw(15) << "Tanggal" << endl;
    cout << string(85, '=') << endl;

    int index = 1;
    for (const Task *task : TodoTasks)
    {
        cout << left << setw(5) << index++
             << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori)
             << left << setw(15) << task->tanggal
             << endl;
    }
}

vector<Task *> concatenateTaskLists(const vector<Task *> &TodoTasks, const vector<Task *> &CompletedTasks) {
    vector<Task *> AllTasks;
    AllTasks.reserve(TodoTasks.size() + CompletedTasks.size());
    AllTasks.insert(AllTasks.end(), TodoTasks.begin(), TodoTasks.end());
    AllTasks.insert(AllTasks.end(), CompletedTasks.begin(), CompletedTasks.end());
    return AllTasks;
}

void displaySubtasks(Task *task, int level = 0) {
    if (task->subtasks.empty()) {
        cout << "Tidak ada subtask untuk task ini." << endl;
        return;
    }

    cout << "Subtasks dari task '" << task->nama << "':" << endl;
    for (size_t i = 0; i < task->subtasks.size(); ++i) {
        cout << i + 1 << ". " << task->subtasks[i]->nama << endl;
    }
}

void markSubtaskAsDone(vector<Task *> &tasks) {
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    
    displayMainTasks(tasks);
    
    cout << "Pilih nomor task yang memiliki subtask yang ingin ditandai selesai: ";
    int taskNumber;
    cin >> taskNumber;
    cin.ignore();

    if (taskNumber < 1 || taskNumber > tasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *task = tasks[taskNumber - 1];
    
    if (task->subtasks.empty()) {
        cout << "Task ini tidak memiliki subtask." << endl;
        return;
    }
    
    displaySubtasks(task);
    
    cout << "Pilih nomor subtask yang ingin ditandai selesai: ";
    int subtaskNumber;
    cin >> subtaskNumber;
    cin.ignore();

    if (subtaskNumber < 1 || subtaskNumber > task->subtasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *subtask = task->subtasks[subtaskNumber - 1];
    subtask->status = DONE;
    cout << "Subtask '" << subtask->nama << "' berhasil ditandai sebagai selesai." << endl;

    bool allSubtasksDone = true;
    for (Task *sub : task->subtasks) {
        if (sub->status != DONE) {
            allSubtasksDone = false;
            break;
        }
    }

    if (allSubtasksDone) {
        task->status = DONE;
        cout << "Semua subtask dari task '" << task->nama << "' telah selesai. Task ini juga ditandai sebagai selesai." << endl;
    }
}

void deleteSubtask(Task *task) {
    displaySubtasks(task);

    cout << "Pilih nomor subtask yang ingin dihapus: ";
    int nomorSubtask;
    cin >> nomorSubtask;
    cin.ignore();

    if (nomorSubtask < 1 || nomorSubtask > task->subtasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *subtaskToDelete = task->subtasks[nomorSubtask - 1];
    delete subtaskToDelete;
    task->subtasks.erase(task->subtasks.begin() + (nomorSubtask - 1));

    cout << "Subtask berhasil dihapus." << endl;
}

void deleteSubtaskMenu(vector<Task *> &tasks) {
    if(tasks.empty()){
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayMainTasks(tasks);

    cout << "Pilih nomor task yang ingin dihapus subtasknya: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > tasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *task = tasks[nomorTask - 1];

    deleteSubtask(task);
}

int main()
{
    banner();
    while (true)
    {
        TodoTasks = TodoTasks;
        cout << "\n========  MENU UTAMA  ========\n";
        cout << "1. Tambah Task" << endl;
        cout << "2. Tampilkan Task" << endl;
        cout << "3. Tandai Task Selesai" << endl;
        cout << "4. Hapus Task" << endl;
        cout << "5. Keluar" << endl;
        cout << "Pilih opsi: ";

        int opsi;
        cin >> opsi;
        cin.ignore();

        switch (opsi)
        {
            case 1:
                while (true)
                {
                    cout << "\n========  MENU TAMBAH TASK  ========\n";
                    cout << "1. Tambah Task" << endl;
                    cout << "2. Tambahkan Subtask" << endl;
                    cout << "Pilih opsi: ";
                    cin >> opsi;
                    cin.ignore();
                    if(cin.fail() || opsi < 0 || opsi > 2){
                        handleInvalidInput();
                    }
                    else{
                        break;
                    }
                }
                if(opsi == 1){
                    createTask();
                }
                else if(opsi == 2){
                    addSubtask(TodoTasks);
                }
                break;
            case 2:
                while (true)
                {
                    cout << "\n========  MENU TAMPILKAN TASK  ========\n";
                    cout << "1. Tampilkan Semua Task" << endl;
                    cout << "2. Tampilkan Task Berdasarkan Kategori" << endl;
                    cout << "3. Tampilkan Todo Task" << endl;
                    cout << "4. Tampilkan Task Selesai" << endl;
                    cout << "Pilih opsi: ";
                    cin >> opsi;
                    cin.ignore();
                    if(cin.fail() || opsi < 0 || opsi > 4){
                        handleInvalidInput();
                    }
                    else{
                        break;
                    }
                }
                if(opsi == 1){
                    displayAllTasks(concatenateTaskLists(TodoTasks, CompletedTasks));
                }
                else if(opsi == 2){
                    displayTasksByCategory(concatenateTaskLists(TodoTasks, CompletedTasks));
                }
                else if(opsi == 3){
                    displayMainTasks(TodoTasks);
                }
                else if(opsi == 4){
                    displayMainTasks(CompletedTasks);
                }
                break;
            case 3:
                while (true)
                {
                    cout << "\n========  MENU Selesaikan TASK  ========\n";        
                    cout << "1. Tandai Selesai Task Pertama di Queue" << endl;
                    cout << "2. Tandai Task selesai" << endl;
                    cout << "3. Tandai Subtask selesai" << endl;
                    cout << "Pilih opsi: ";
                    cin >> opsi;
                    cin.ignore();
                    if(cin.fail() || opsi < 0 || opsi > 3){
                        handleInvalidInput();
                    }
                    else{
                        break;
                    }
                }
                if(opsi == 1){
                    markFirstQueueAsDone(TodoTasks, CompletedTasks);
                }
                else if(opsi == 2){
                    markAsDone(TodoTasks, CompletedTasks);
                }
                else if(opsi == 3){
                    markSubtaskAsDone(TodoTasks);
                }
                break;
            case 4:
                while (true)
                {
                    cout << "\n========  MENU Selesaikan TASK  ========\n";        
                    cout << "1. Hapus Task Terakhir" << endl;
                    cout << "2. Hapus Task" << endl;
                    cout << "3. Hapus Subtask" << endl;
                    cout << "Pilih opsi: ";
                    cin >> opsi;
                    cin.ignore();
                    if(cin.fail() || opsi < 0 || opsi > 3){
                        handleInvalidInput();
                    }
                    else{
                        break;
                    }
                }
                if(opsi == 1){
                    deleteLastTask(TodoTasks);
                }
                else if(opsi == 2){
                    deleteTask(TodoTasks);
                }
                else if(opsi == 3){
                    deleteSubtaskMenu(TodoTasks);
                }
                break;
            case 5:
                cout << "Keluar program" << endl;
                exit(EXIT_SUCCESS);
            default:
                handleInvalidInput();
                break;
        }
    }

    for (Task *task : TodoTasks)
    {
        delete task;
    }

    return 0;
}
