#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

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

    Task(string n, string d, Status s, Kategori k) : nama(n), deskripsi(d), status(s), kategori(k) {}
};

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
    string nama, deskripsi;

    cout << "\n========  MEMBUAT TASK  ========\n";
    cout << "Masukkan Nama Task: ";
    getline(cin, nama);
    cout << "Masukkan Deskripsi Task: ";
    getline(cin, deskripsi);

    Status status = TODO;
    Kategori kategori = inputKategori();

    cout << "Task berhasil dibuat!\n";
    return new Task(nama, deskripsi, status, kategori);
}

void displayTasks(const vector<Task *> &tasks)
{
    cout << "\n";
    cout << left << setw(20) << "Nama"
         << left << setw(30) << "Deskripsi"
         << left << setw(10) << "Status"
         << left << setw(15) << "Kategori" << endl;

    cout << string(75, '=') << endl;

    for (const Task *task : tasks)
    {
        cout << left << setw(20) << task->nama
             << left << setw(30) << task->deskripsi
             << left << setw(10) << statusToString(task->status)
             << left << setw(15) << kategoriToString(task->kategori) << endl;
    }
}

void displayTasksByCategory(const vector<Task *> &tasks, Kategori category)
{
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

void displayAllTasks(const vector<Task *> &tasks)
{
    displayTasks(tasks);
}

int main()
{
    vector<Task *> tasks;
    banner();
    while (true)
    {
        cout << "\n========  MENU UTAMA  ========\n";
        cout << "1. Tambah Task" << endl;
        cout << "2. Tampilkan Task Berdasarkan Kategori" << endl;
        cout << "3. Tampilkan Semua Task" << endl;
        cout << "4. Keluar" << endl;
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
            cout << "Pilih Kategori untuk Ditampilkan (0: WORK, 1: PERSONAL, 2: OTHER): ";
            int kategoriPilihan;
            cin >> kategoriPilihan;
            cin.ignore();
            Kategori kategori = static_cast<Kategori>(kategoriPilihan);
            displayTasksByCategory(tasks, kategori);
            break;
        }
        case 3:
        {
            displayAllTasks(tasks);
            break;
        }
        case 4:
        {
            cout << "Keluar program." << endl;
            break;
        }
        default:
        {
            cout << "Opsi tidak valid." << endl;
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
