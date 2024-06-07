#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <cstdlib>

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
    string tanggal;

    Task(string n, string d, Status s, Kategori k, string t) : nama(n), deskripsi(d), status(s), kategori(k), tanggal(t) {}
};
vector<Task *> taskStack;

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
    int pilihan;
    cout << "Pilih Kategori (0: WORK, 1: PERSONAL, 2: OTHER): ";
    cin >> pilihan;
    cin.ignore();
    switch (pilihan)
    {
    case 1:
        return PERSONAL;
    case 2:
        return OTHER;
    default:
        return WORK;
    }
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

    cout << "Task berhasil dibuat!\n";
    Task *newTask = new Task(nama, deskripsi, status, kategori, tanggal);
    taskStack.push_back(newTask);
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

void displayTasks(const vector<Task *> &tasks, int level = 0)
{
    string indent(level * 4, ' ');

    if (level == 0)
    {
        cout << "\n";
        cout << left << setw(5) << "No"
             << left << setw(20) << "Nama"
             << left << setw(30) << "Deskripsi"
             << left << setw(10) << "Status"
             << left << setw(15) << "Kategori"
             << left << setw(15) << "Tanggal" << endl;
        cout << string(85, '=') << endl;
    }

    int index = 1;
    for (const Task *task : tasks)
    {
        cout << indent << left << setw(5) << index++
             << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori)
             << left << setw(15) << task->tanggal
             << endl;

        if (!task->subtasks.empty())
        {
            displayTasks(task->subtasks, level + 1);
        }
    }
}

void displayAllTasks(const vector<Task *> &tasks)
{
    displayTasks(tasks);
}

void displayTasksByCategory(const vector<Task *> &tasks, Kategori category)
{
    vector<Task *> filteredTasks;
    for (const Task *task : tasks)
    {
        if (task->kategori == category)
        {
            filteredTasks.push_back(const_cast<Task *>(task));
            for (const Task *subtask : task->subtasks)
            {
                if (subtask->kategori == category)
                {
                    filteredTasks.push_back(const_cast<Task *>(subtask));
                }
            }
        }
    }
    displayTasks(filteredTasks);
}

void displayTodoTasks(const vector<Task *> &tasks)
{
    cout << "\n";
    cout << left << setw(5) << "No"
         << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(15) << "Kategori"
         << left << setw(15) << "Tanggal"
         << endl;

    cout << string(85, '=') << endl;

    int index = 1;

    for (const Task *task : tasks)
    {
        if (task->status == TODO)
        {
            cout << left << setw(5) << index
                 << left << setw(20) << task->nama
                 << left << setw(30) << task->deskripsi
                 << left << setw(15) << kategoriToString(task->kategori)
                 << left << setw(15) << task->tanggal
                 << endl;
            index++;
        }
    }
}

void addSubtask(vector<Task *> &tasks)
{
    displayAllTasks(tasks);

    cout << "Pilih nomor task yang ingin ditambahkan subtask: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > tasks.size())
    {
        cout << "Nomor task tidak valid." << endl;
        return;
    }

    Task *task = tasks[nomorTask - 1];
    Task *newSubtask = createSubtask();

    // Set kategori subtask sesuai dengan parent
    newSubtask->kategori = task->kategori;

    task->subtasks.push_back(newSubtask);

    cout << "Subtask berhasil ditambahkan ke task '" << task->nama << "'." << endl;
}

void markAsDone(vector<Task *> &tasks)
{
    if (taskStack.empty())
    {
        cout << "Stack kosong." << endl;
        return;
    }

    Task *taskTerbaru = taskStack.back();
    taskTerbaru->status = DONE;
    cout << "Task pertama yaitu '" << taskTerbaru->nama << "' berhasil ditandai sebagai selesai." << endl;
    taskStack.pop_back();
}

void deleteTask(vector<Task *> &tasks)
{
    displayAllTasks(tasks);

    cout << "Pilih nomor task yang ingin dihapus: ";
    int nomorTask;
    cin >> nomorTask;
    cin.ignore();

    if (nomorTask < 1 || nomorTask > tasks.size())
    {
        cout << "Nomor task tidak valid." << endl;
        return;
    }

    delete tasks[nomorTask - 1];
    tasks.erase(tasks.begin() + (nomorTask - 1));

    cout << "Task berhasil dihapus." << endl;
}

void displayStack()
{
    if (taskStack.empty())
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
    for (const Task *task : taskStack)
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

int main()
{
    vector<Task *> tasks;
    banner();
    while (true)
    {
        cout << "\n========  MENU UTAMA  ========\n";
        cout << "1. Tambah Task" << endl;
        cout << "2. Tambahkan Subtask" << endl;
        cout << "3. Tampilkan Semua Task" << endl;
        cout << "4. Tampilkan Task Berdasarkan Kategori" << endl;
        cout << "5. Tampilkan Todo Task" << endl;
        cout << "6. Tandai Task Selesai" << endl;
        cout << "7. Hapus Task" << endl;
        cout << "8. Keluar" << endl;
        cout << "Pilih opsi: ";

        int opsi;
        cin >> opsi;
        cin.ignore();

        switch (opsi)
        {
        case 1:
        {
            Task *newTask = createTask();
            tasks.push_back(newTask);
            break;
        }
        case 2:
        {
            addSubtask(tasks);
            break;
        }
        case 3:
        {
            displayAllTasks(tasks);
            break;
        }
        case 4:
        {
            Kategori kategori = inputKategori();
            displayTasksByCategory(tasks, kategori);
            break;
        }
        case 5:
        {
            displayTodoTasks(tasks);
            break;
        }
        case 6:
        {
            markAsDone(tasks);
            break;
        }
        case 7:
        {
            deleteTask(tasks);
            break;
        }
        case 8:
        {
            cout << "Keluar program" << endl;
            exit(EXIT_SUCCESS);
        }
        default:
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opsi tidak valid" << endl;
            break;
        }
        }
    }

    for (Task *task : tasks)
    {
        delete task;
    }

    return 0;
}
