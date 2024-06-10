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

// Menangani input yang tidak valid dengan membersihkan buffer input dan menampilkan pesan kesalahan.
void handleInvalidInput(){
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "\x1b[31mInvalid Input\x1b[0m" << endl;
}

// Mengonversi nilai enumerasi Status menjadi string yang sesuai (TODO atau DONE).
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

// Mengonversi nilai enumerasi Kategori menjadi string yang sesuai (WORK, PERSONAL, atau OTHER).
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

// Mengembalikan string yang berisi tanggal hari ini dalam format "YYYY-MM-DD".
string getTanggalHariIni()
{
    time_t t = time(0);
    tm *now = localtime(&t);
    char buffer[11];
    strftime(buffer, 11, "%Y-%m-%d", now);
    return string(buffer);
}

// Meminta pengguna untuk memilih kategori (WORK, PERSONAL, atau OTHER) dan mengembalikan nilai enumerasi Kategori yang sesuai.
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

// Memeriksa apakah sebuah string mengandung setidaknya satu huruf alfabet.
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



// Fungsi ini menerima vector dari tugas (tasks) dan tingkat indentasi (level) sebagai argumen.
// Pertama, fungsi akan menampilkan informasi tentang tugas yang ada pada tingkat saat ini dengan menggunakan indentasi sesuai dengan levelnya (Kata yang menjorok).
// Kemudian, fungsi akan memeriksa apakah tugas tersebut memiliki subtask atau tidak.
// Jika memiliki subtask, fungsi akan memanggil dirinya sendiri (rekursi) untuk menampilkan subtask dengan tingkat indentasi yang lebih dalam (level + 1).
// Proses ini akan terus berlanjut hingga semua subtask dari setiap tugas ditampilkan.
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



// Fungsi ini menerima vector dari tugas utama (tasks) sebagai argumen.
// Pertama, fungsi akan menampilkan informasi tentang setiap tugas utama seperti nama, deskripsi, status, kategori, tanggal, dan dependensi.
// Fungsi ini tidak menampilkan subtask dari setiap tugas utama, hanya informasi dasar tentang tugas utama itu sendiri.
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



// Fungsi menerima daftar tasksList.
// Jika tasksList kosong, fungsi mengembalikan daftar dependency kosong.
// Pengguna diminta untuk menentukan apakah task memiliki dependency.
// Jika ya, fungsi menampilkan daftar task utama dan meminta pengguna memilih nomor task yang menjadi dependency.
// Task yang dipilih ditambahkan ke daftar dependencies.
// Daftar dependencies dikembalikan setelah pengguna selesai memilih atau memilih untuk tidak memiliki dependency.
// Pesan kesalahan ditampilkan jika input tidak valid.
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



// Meminta pengguna memasukkan nama dan deskripsi task.
// Mengambil tanggal hari ini sebagai tanggal task dibuat.
// Menentukan status task sebagai TODO dan kategori task dengan memanggil fungsi inputKategori.
// Membersihkan input buffer menggunakan cin.clear() dan cin.ignore() untuk menghindari masalah input.
// Membuat objek Task baru dengan informasi yang dimasukkan pengguna.
// Menentukan dependency task dengan memanggil fungsi createDependency untuk mendapatkan daftar dependency dari TodoTasks.
// Menambahkan task baru ke dalam TodoTasks.
// Mengembalikan pointer ke task yang baru dibuat.
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



// Meminta pengguna memasukkan nama dan deskripsi untuk subtask.
// Mengambil tanggal hari ini sebagai tanggal pembuatan subtask.
// Menetapkan status subtask sebagai TODO dan kategori subtask sebagai WORK.
// Membuat objek Task baru untuk subtask dengan informasi yang dimasukkan pengguna.
// Mengembalikan pointer ke subtask yang baru dibuat.
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



// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Meminta pengguna memilih kategori menggunakan fungsi inputKategori.
// Membuat vektor filteredTasks untuk menyimpan task yang memiliki kategori yang sesuai.
// Melakukan iterasi melalui setiap task dalam vektor tasks.
// Memeriksa apakah kategori task sesuai dengan kategori yang dipilih pengguna.
// Jika sesuai, menambahkan task tersebut ke dalam vektor filteredTasks.
// Memanggil fungsi displayTasks untuk menampilkan daftar task yang telah difilter berdasarkan kategori yang dipilih.
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


// Membuat vektor AllTasks untuk menyimpan hasil penggabungan.
// Mengalokasikan memori yang cukup untuk AllTasks dengan menggunakan reserve untuk menghindari alokasi ulang.
// Menyalin elemen-elemen dari TodoTasks ke AllTasks menggunakan insert.
// Menyalin elemen-elemen dari CompletedTasks ke AllTasks menggunakan insert.
// Mengembalikan AllTasks yang telah digabungkan.
// karena di concatenate, jadi yang paling pertama selalu berstatus TODO dan yang belakang selalu berstatus DONE
vector<Task *> concatenateTaskLists(const vector<Task *> &TodoTasks, const vector<Task *> &CompletedTasks) {
    vector<Task *> AllTasks;
    AllTasks.reserve(TodoTasks.size() + CompletedTasks.size());
    AllTasks.insert(AllTasks.end(), TodoTasks.begin(), TodoTasks.end());
    AllTasks.insert(AllTasks.end(), CompletedTasks.begin(), CompletedTasks.end());
    return AllTasks;
}



// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Menampilkan daftar task utama menggunakan fungsi displayMainTasks.
// Meminta pengguna memilih nomor task yang ingin ditambahkan subtask.
// Memeriksa apakah nomor task yang dimasukkan pengguna valid.
// Mengakses task yang dipilih menggunakan nomor task.
// Membuat subtask baru dengan menggunakan fungsi createSubtask.
// Menetapkan kategori subtask sesuai dengan kategori task utama.
// Menambahkan subtask baru ke dalam vektor subtasks dari task utama.
// Memberikan konfirmasi bahwa subtask berhasil ditambahkan ke dalam task utama.
void addSubtask(vector<Task *> &todoTasks) {
    if (todoTasks.empty()) {
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayMainTasks(todoTasks);

    cout << "Pilih nomor task yang ingin ditambahkan subtask: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > todoTasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *task = todoTasks[nomorTask - 1];
    Task *newSubtask = createSubtask();

    // Set kategori subtask sesuai dengan parent
    newSubtask->kategori = task->kategori;

    // Tambahkan subtask ke task yang ada di todoTasks
    todoTasks[nomorTask - 1]->subtasks.push_back(newSubtask);

    cout << "Subtask berhasil ditambahkan ke task '" << task->nama << "'." << endl;
}




// Menetapkan status task yang diberikan sebagai DONE.
// Mengulangi langkah 1 untuk setiap subtask dalam vektor subtasks dari task tersebut.
void markAllSubtaskDone(Task *task) {
    task->status = DONE;

    // Set semua subtasknya ke done
    for (Task *subtask : task->subtasks) {
        markAllSubtaskDone(subtask);
    }
}


// Memeriksa apakah vektor TodoTasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Mengakses task pertama dalam vektor TodoTasks.
// Menandai task tersebut dan semua subtasknya sebagai selesai menggunakan fungsi markAllSubtaskDone.
// Menghapus task yang telah selesai dari vektor TodoTasks.
// Menambahkan task yang selesai ke dalam vektor completedTasks.
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


// Memeriksa setiap dependensi dari task.
// Jika ada dependensi yang belum selesai, mengembalikan false.
// Jika semua dependensi sudah selesai, mengembalikan true.
bool canBeMarkedAsDone(Task *task) {
    for (Task *dep : task->dependency) {
        if (dep->status != DONE) {
            return false;
        }
    }
    return true;
}



// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task yang tersedia untuk ditandai sebagai selesai.
// Menampilkan daftar task menggunakan fungsi displayTasks.
// Meminta pengguna memilih nomor task yang ingin ditandai sebagai selesai.
// Memeriksa apakah nomor task yang dimasukkan pengguna valid.
// Mengakses task yang dipilih menggunakan nomor task.
// Memeriksa apakah task dapat ditandai sebagai selesai menggunakan fungsi canBeMarkedAsDone.
// Jika task tidak dapat ditandai sebagai selesai, mencetak pesan yang sesuai.
// Jika task dapat ditandai sebagai selesai, menandai task dan semua subtasknya sebagai selesai menggunakan markAllSubtaskDone.
// Menghapus task yang telah ditandai sebagai selesai dari vektor tasks.
// Menambahkan task yang selesai ke dalam vektor completedTasks.
// Memberikan konfirmasi bahwa task dan subtasknya berhasil ditandai sebagai selesai.
void markAsDone(vector<Task *> &tasks, vector<Task *> &completedTasks) {
    if (tasks.empty()) {
        cout << "Tidak ada task yang tersedia untuk ditandai sebagai selesai." << endl;
        return;
    }

    displayMainTasks(tasks);

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







// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Menampilkan daftar task menggunakan fungsi displayTasks.
// Meminta pengguna memilih nomor task yang ingin dihapus.
// Memeriksa apakah nomor task yang dimasukkan pengguna valid.
// Menghapus task dari vektor tasks menggunakan delete untuk mencegah kebocoran memori.
// Menghapus elemen dari vektor tasks menggunakan erase.
// Memberikan konfirmasi bahwa task berhasil dihapus.
void deleteTask(vector<Task *> &todoTasks, vector<Task *> &completedTasks) {
    vector<Task *> allTasks = concatenateTaskLists(todoTasks, completedTasks);

    if (allTasks.empty()) {
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayTasks(allTasks);

    cout << "Pilih nomor task yang ingin dihapus: ";
    int nomorTask;
    cin >> nomorTask;

    if (cin.fail() || nomorTask < 1 || nomorTask > allTasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *taskToDelete = allTasks[nomorTask - 1];
    delete taskToDelete;

    // Hapus dari TodoTasks atau CompletedTasks
    if (nomorTask <= todoTasks.size()) {
        todoTasks.erase(todoTasks.begin() + (nomorTask - 1));
    } else {
        completedTasks.erase(completedTasks.begin() + (nomorTask - todoTasks.size() - 1));
    }

    cout << "Task berhasil dihapus." << endl;
}


// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task yang bisa dihapus.
// Menghapus task terakhir dari vektor tasks menggunakan pop_back.
// Memberikan konfirmasi bahwa task terakhir berhasil dihapus.
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




// Memeriksa apakah task memiliki subtask. Jika tidak, mencetak pesan bahwa tidak ada subtask untuk task tersebut.
// Jika task memiliki subtask, mencetak judul "Subtasks dari task 'nama_task'".
// Mengulangi langkah 4 untuk setiap subtask dalam vektor subtasks.
// Mencetak nomor dan nama subtask.
void displaySubtasks(Task *task, vector<int> &todoSubtaskIndices, int level = 0) {
    todoSubtaskIndices.clear();

    if (task->subtasks.empty()) {
        cout << "Tidak ada subtask untuk task ini." << endl;
        return;
    }
    
    cout << "Todo Subtasks dari task '" << task->nama << "':" << endl;
    for (size_t i = 0; i < task->subtasks.size(); ++i) {
        if (task->subtasks[i]->status == TODO) {
            cout << todoSubtaskIndices.size() + 1 << ". " << task->subtasks[i]->nama << endl;
            todoSubtaskIndices.push_back(i);
        }
    }

    if (todoSubtaskIndices.empty()) {
        cout << "Tidak ada subtask yang belum selesai untuk task ini." << endl;
    }
}





// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Menampilkan daftar task utama menggunakan fungsi displayMainTasks.
// Meminta pengguna memilih nomor task yang memiliki subtask yang ingin ditandai sebagai selesai.
// Memeriksa apakah nomor task yang dimasukkan pengguna valid.
// Mengakses task yang dipilih menggunakan nomor task.
// Memeriksa apakah task memiliki subtask. Jika tidak, mencetak pesan bahwa task tersebut tidak memiliki subtask.
// Jika task memiliki subtask, menampilkan daftar subtask menggunakan fungsi displaySubtasks.
// Meminta pengguna memilih nomor subtask yang ingin ditandai sebagai selesai.
// Memeriksa apakah nomor subtask yang dimasukkan pengguna valid.
// Mengakses subtask yang dipilih menggunakan nomor subtask.
// Menetapkan status subtask tersebut sebagai DONE.
// Memberikan konfirmasi bahwa subtask berhasil ditandai sebagai selesai.
// Memeriksa apakah semua subtask dari task telah selesai. Jika ya, menetapkan status task tersebut sebagai DONE dan memberikan konfirmasi bahwa task juga ditandai sebagai selesai.
void markSubtaskAsDone(vector<Task *> &tasks, vector<Task *> &completedTasks) {
    if (tasks.empty()) {
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

    vector<int> todoSubtaskIndices;
    displaySubtasks(task, todoSubtaskIndices);
    
    if (todoSubtaskIndices.empty()) {
        return;
    }

    cout << "Pilih nomor subtask yang ingin ditandai selesai: ";
    int subtaskNumber;
    cin >> subtaskNumber;
    cin.ignore();

    if (subtaskNumber < 1 || subtaskNumber > todoSubtaskIndices.size()) {
        handleInvalidInput();
        return;
    }

    int actualIndex = todoSubtaskIndices[subtaskNumber - 1];
    Task *subtask = task->subtasks[actualIndex];
    subtask->status = DONE;
    cout << "Subtask '" << subtask->nama << "' berhasil ditandai sebagai selesai." << endl;

    bool allSubtasksDone = true;
    for (Task *sub : task->subtasks) {
        if (sub->status != DONE) {
            allSubtasksDone = false;
            break;
        }
    }

    allSubtasksDone = canBeMarkedAsDone(task);

    if (allSubtasksDone) {
        task->status = DONE;
        cout << "Semua subtask dari task '" << task->nama << "' telah selesai. Task ini juga ditandai sebagai selesai." << endl;

        tasks.erase(tasks.begin() + (taskNumber - 1));
        completedTasks.push_back(task);
    }
}





// Menampilkan daftar subtask menggunakan fungsi displaySubtasks.
// Meminta pengguna memilih nomor subtask yang ingin dihapus.
// Memeriksa apakah nomor subtask yang dimasukkan pengguna valid.
// Menghapus subtask yang dipilih menggunakan delete untuk mencegah kebocoran memori.
// Menghapus elemen dari vektor subtasks milik task menggunakan erase.
// Memberikan konfirmasi bahwa subtask berhasil dihapus.
void deleteSubtask(Task *task) {
    vector<int> todoSubtaskIndices;
    displaySubtasks(task, todoSubtaskIndices);

    if (todoSubtaskIndices.empty()) {
        return;
    }

    cout << "Pilih nomor subtask yang ingin dihapus: ";
    int nomorSubtask;
    cin >> nomorSubtask;
    cin.ignore();

    if (nomorSubtask < 1 || nomorSubtask > todoSubtaskIndices.size()) {
        handleInvalidInput();
        return;
    }

    int actualIndex = todoSubtaskIndices[nomorSubtask - 1];
    Task *subtaskToDelete = task->subtasks[actualIndex];
    delete subtaskToDelete;
    task->subtasks.erase(task->subtasks.begin() + actualIndex);

    cout << "Subtask berhasil dihapus." << endl;
}






// Memeriksa apakah vektor tasks kosong. Jika ya, mencetak pesan bahwa tidak ada task saat ini.
// Menampilkan daftar task menggunakan fungsi displayMainTasks.
// Meminta pengguna memilih nomor task yang ingin dihapus subtasknya.
// Memeriksa apakah nomor task yang dimasukkan pengguna valid.
// Mendapatkan pointer ke task yang dipilih.
// Memanggil fungsi deleteSubtask untuk menghapus subtask dari task yang dipilih.
void deleteSubtaskMenu(vector<Task *> &todoTasks, vector<Task *> &completedTasks) {
    vector<Task *> allTasks = concatenateTaskLists(todoTasks, completedTasks);

    if (allTasks.empty()) {
        cout << "Belum ada task saat ini" << endl;
        return;
    }
    displayMainTasks(allTasks);

    cout << "Pilih nomor task yang ingin dihapus subtasknya: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > allTasks.size()) {
        handleInvalidInput();
        return;
    }

    Task *task = allTasks[nomorTask - 1];
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
                    displayTasks(concatenateTaskLists(TodoTasks, CompletedTasks));
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
                    markSubtaskAsDone(TodoTasks, CompletedTasks);
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
                else if (opsi == 2) {
                    deleteTask(TodoTasks, CompletedTasks);
                }
                else if (opsi == 3) {
                    deleteSubtaskMenu(TodoTasks, CompletedTasks);
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
