#include <iostream>
#include <fstream>
#include <conio.h>
#include <string.h>
#include <typeinfo>
using namespace std;

int max_length = 10;

enum class ct
{
    base_item,
    furniture,
    clothes,
    belt,
    trousers,
    table,
    chair,
};

class Error
{
    public:
        string s;
        int id;
    Error(string s, int id)
    {
        this->s = s;
        this->id = id;
    };
};

class item  // базовый класс
{
    private:
        int x = 0;
        int y = 0;
        char name[10];

    protected:

    public:
        static inline int count = 0;
        static inline item* list_of_id [200]; 
        static inline int max_elem = 0;

        static void output(string s);
        static void massive();
        static void clear();
        

        //атрибуты, нужные для манипуляций с классами//
        int id = 0; 
        ct type;

        void set_x (int x)
        {
            this->x = x;
        };

        void set_y (int y)
        {
            this->y = y;
        };

        void set_name (string name)
        {
            char charr[max_length];
            name.copy(charr, name.length());
            for (int i = 0; i < max_length; i++)
            {
                this->name[i] = charr[i];
            };
        };

        virtual void get ()
        {
            cout << this->x << ' ' << this->y;
            cout << ' ' << this->name << endl;
        };

        int get_x ()
        {
            return this->x;
        };

        int get_y ()
        {
            return this->y;
        };

        string get_name ()
        {
            return this->name;
        };

        item()
        {
            type = ct::base_item;
            this->id = max_elem;
            list_of_id[id] = this;
            max_elem++;
        };

        ~item()
        {
            list_of_id[id] = 0;
            id--;
        };

        char* to_char_point (item* pitem)
        {
            try
            {
                return reinterpret_cast<char*>(pitem);
            }
            catch(Error & e)
            {
            cout << e.s << endl;
            };    
        };

        virtual void get(fstream& file); //извлечение объекта из файла

        virtual void put(string fname); //помещение объекта в файл

        void rearrange();

};

class furniture: public item //мебель
{
    private:

    public:
    int size = 0;

    furniture()
    {
        type = ct::furniture;
    };

    virtual void set_size(int size)
    {
        this->size = size;
    };

    virtual void get()
    {
        cout << this->size << " ";
        cout << this->get_name() << endl;
    };
};

class clothes: public item //одежда
{
    private:

    public:
    char colour[10];
    
    clothes()
    {
        type = ct::clothes;
        string clname = "_no_name_";
        clname.copy(this->colour, clname.length());
    };

    void set_colour (string colour)
    {
        char charr[max_length];
        colour.copy(charr, colour.length());
        for (int i = 0; i < max_length; i++)
        {
            this->colour[i] = charr[i];
        };
    };

    virtual void get()
    {
        cout << this->colour << " ";
        cout << this->get_name() << endl;
    };
};

class belt: public clothes //ремни
{
    private:

    public:
    int length = 0;
    
    belt()
    {
        type = ct::belt;
    };

    void set_length (int length)
    {
        this->length = length;
    };

    virtual void get()
    {
        cout << "Belt" << " ";
        cout << this->colour << " ";
        cout << this->length << " ";
        cout << this->get_name() << endl;
    };
};

class trousers: public clothes // Брюки
{
    private:

    public:
    int size = 0;
    
    trousers()
    {
        type = ct::trousers;
    };

    void set_size (int size)
    {
        this->size = size;
    };

    void get()
    {
        cout << "Trousers" << " ";
        cout << this->colour << " ";
        cout << this->size << " ";
        cout << this->get_name() << endl;
    };
};

class table: public furniture // столы
{
    private:

    public:
    int leg_number = 0;


    table()
    {
        type = ct::table;
    };

    void set_leg_number (int leg_number)
    {
        this->leg_number = leg_number;
    };

    void get()
    {
        cout << "Table" << " ";
        cout << this->leg_number << " ";
        cout << this->size << " ";
        cout << this->get_name() << endl;
    };
};

class chair: public furniture // стулья, кресла
{
    private:

    public:
    int height = 0;
    
    chair()
    {
        type = ct::chair;
    };

    void set_height (int height)
    {
        this->height = height;
    };

    void get()
    {
        cout << "Chair" << " ";
        cout << this->height << " ";
        cout << this->size << " ";
        cout << this->get_name() << endl;
    };
};

class cart: public item 
{
    private:

    public:

    template<class t>
    void to_cart(t* a, string c)
    {
        a->put(c);
    };
};

        void item::put(string fname)
        {
            int obj_size = 0;
            if(typeid(*this)==typeid(chair)) obj_size=sizeof(chair);
            if(typeid(*this)==typeid(table)) obj_size=sizeof(table);
            if(typeid(*this)==typeid(belt)) obj_size=sizeof(belt);
            if(typeid(*this)==typeid(trousers)) obj_size=sizeof(trousers);
            fstream file;
            file.open(fname, ios::app | ios::binary);
            if(!file.is_open())
            {
                cout << "Failed open file";
            }
            else
            {   
                file.write(reinterpret_cast<char*>(&type), sizeof(char));
                file.write(reinterpret_cast<char*>(&obj_size), sizeof(int));
                file.write(reinterpret_cast<char*>(this), obj_size);
            };
            file.close();
        };
        void item::get(fstream& file)
        {
            int obj_size = 0;
            if(typeid(*this)==typeid(chair)) obj_size=sizeof(chair);
            if(typeid(*this)==typeid(table)) obj_size=sizeof(table);
            if(typeid(*this)==typeid(belt)) obj_size=sizeof(belt);
            if(typeid(*this)==typeid(trousers)) obj_size=sizeof(trousers);
            file.read(reinterpret_cast<char*>(this), obj_size); //выделить память для того, чтобы считывать значения, иначе обращается не в ту область памяти
        };


int main()
{ 
    int selection = 999;
    string buf;
    int temp;
    string s = "c:\\projects_cpp_vs\\class_file"; // назначение путей файлов склада и корзины
    string c = "c:\\projects_cpp_vs\\cart_file";
    fstream file;
    file.open(s, ios::out | ios::binary); // перезапись и заполнение файлов продуктами, что были на складе
    file.close();
    file.open(c, ios::out | ios::binary);
    file.close();
    cart cart;
    belt* Classic = new belt();
    Classic->set_colour("Black");
    Classic->set_name("Classic");
    Classic->set_length(20);
    trousers* Boxers = new trousers();
    Boxers->set_colour("Purple");
    Boxers->set_name("Boxers");
    Boxers->set_size(56);
    table* Ikea = new table();
    Ikea->set_name("Glass");
    Ikea->set_leg_number(4);
    Ikea->set_size(300);
    chair* Gaming = new chair();
    Gaming->set_name("Gamers");
    Gaming->set_height(150);
    Gaming->set_size(50);
    Classic->put(s);
    Boxers->put(s);
    Ikea->put(s);
    Gaming->put(s);
    delete(Classic);
    delete(Boxers);
    delete(Ikea);
    delete(Gaming);

    while (selection!=0) // реализация пользовательского интерфейса
    {
        for (int i = 0; i < 3000; i++)
            cout << " ";
        cout << "|___MARKETPLACE___|" << endl;
        cout << "1. Administrator mode" << endl;
        cout << "2. Customer mode" << endl;
        cout << "0. Quit" << endl;
        cin >> selection;
        switch (selection)
        {
        case 1:                                //режим администратора
        {   while(selection!=0)
            {
                for (int i = 0; i < 3000; i++)
                    cout << " ";
                cout << "|___ADMINISTRATOR___|" << endl;
                cout << "1. Check stash" << endl;
                cout << "2. Add item" << endl;
                cout << "0. Back" << endl;
                cin >> selection;
                switch (selection)
                {
                case 0:
                    exit;
                    break;

                case 1:                                 // проверка склада   
                    cout << endl << "Stash:" << endl;
                    Gaming->output(s);
                    Gaming->rearrange();
                    Ikea->massive();
                    cout << endl;
                    Classic->clear();
                    cout << "!Press 1 to continue!" << endl;
                    cin >> selection;
                    break;

                case 2:                                // добавление предмета
                    cout << endl << "Input the type of an item:" << endl;
                    cout << "1. Belt" << endl;
                    cout << "2. Trousers " << endl;
                    cout << "3. Table" << endl;
                    cout << "4. Chair" << endl;
                    cout << "0. Back" << endl;
                    cin >> selection;
                    switch (selection)
                    {
                    case 0:
                        exit;
                        break;

                    case 1:
                        {
                        belt* tbelt = new belt();
                        cout << "Input name (9 or less characters):" << endl;
                        cin >> buf;
                        tbelt->set_name(buf);
                        cout << "Input colour (9 or less characters):" << endl;
                        cin >> buf;
                        tbelt->set_colour(buf);
                        cout << "Input length:" << endl;
                        cin >> temp;
                        tbelt->set_length(temp);
                        cout << "Input cost:" << endl;
                        cin >> temp;
                        tbelt->set_x(temp);
                        tbelt->put(s);
                        delete(tbelt);
                        break;
                        }

                    case 2:
                        {
                        trousers* ttrousers = new trousers();
                        cout << "Input name (9 or less characters):" << endl;
                        cin >> buf;
                        ttrousers->set_name(buf);
                        cout << "Input colour (9 or less characters):" << endl;
                        cin >> buf;
                        ttrousers->set_colour(buf);
                        cout << "Input size:" << endl;
                        cin >> temp;
                        ttrousers->set_size(temp);
                        cout << "Input cost:" << endl;
                        cin >> temp;
                        ttrousers->set_x(temp);
                        ttrousers->put(s);
                        delete(ttrousers);
                        break;
                        }

                    case 3:
                        {
                        table* ttable = new table();
                        cout << "Input name (9 or less characters):" << endl;
                        cin >> buf;
                        ttable->set_name(buf);
                        cout << "Input size:" << endl;
                        cin >> temp;
                        ttable->set_size(temp);
                        cout << "Input number of legs:" << endl;
                        cin >> temp;
                        ttable->set_leg_number(temp);
                        cout << "Input cost:" << endl;
                        cin >> temp;
                        ttable->set_x(temp);
                        ttable->put(s);
                        delete(ttable);
                        break;
                        }

                    case 4:
                        {
                        chair* tchair = new chair();
                        cout << "Input name (9 or less characters):" << endl;
                        cin >> buf;
                        tchair->set_name(buf);
                        cout << "Input size:" << endl;
                        cin >> temp;
                        tchair->set_size(temp);
                        cout << "Input height:" << endl;
                        cin >> temp;
                        tchair->set_height(temp);
                        cout << "Input cost:" << endl;
                        cin >> temp;
                        tchair->set_x(temp);
                        tchair->put(s);
                        delete(tchair);
                        break;
                        }

                    default:
                        cout << "Wrong imput";
                        break;
                    }
                    break;
                
                default:
                    cout << "Incorrect input. Try again." << endl;
                    break;
                }
            }
            selection = 999;
        }
            break;

        case 2:
        {
            while(selection!=0)                                //Режим покупателя
            {
            for (int i = 0; i < 3000; i++)
                cout << " ";
            cout << "|___MARKET___|" << endl;
            cout << "Greetings, dear customer!" << endl;
            cout << "There you can find various" << endl;
            cout << "goods caters your all" << endl;
            cout << "wants and needs." << endl;
            cout << "1. Check shelves" << endl;
            cout << "2. Check cart" << endl;
            cout << "0. Back" << endl;
            cin >> selection;
            switch (selection)
            {
            case 0:
                exit;
                break;
            
            case 1:                                 //Проверка полок
                cout << endl << "Goods avaliable:" << endl;
                Gaming->output(s);
                Gaming->rearrange();
                Ikea->massive();
                cout << endl;
                // Classic->clear();
                cout << "Add anything to your cart?" << endl;
                cout << "Enter number of an item to add, else enter 0 to go to main menu." << endl;
                cin >> selection;
                if (selection == 0)
                {
                    Classic->clear();
                    exit;
                }
                else
                {
                    cart.to_cart(cart.list_of_id[selection], c); // воспринимает все объекты, как базовые, из-за чего выводит некорректно (проверить на дальнейште ошибки)
                    Classic->clear();
                }
                break;

            case 2:                                 //проверка содержимого корзины
                cout << "CART" << endl << endl;
                Gaming->output(c);
                Gaming->rearrange();
                Ikea->massive();
                Classic->clear();
                cout << endl;
                cout << "!Press 2 to continue!" << endl;
                cin >> selection;
                break;

            default:
                break;
            }
            
            };
            selection = 999;    
        }
            break;
        
        case 0:
            exit;
            break;
        
        default:
            cout << "Incorrect input. Try again." << endl;
            break;
        } 
    }
};

void item::output(string s) // вывод объектов из файла с названием S в массив объектов базового класса
        {
            int i = 0;
            int newid = 1;
            char cl;
            int type;
            int point_size;
            int obj_size;
            fstream file;
            file.open(s, ios::in | ios::binary);
            if(!file.is_open())
            {
                cout << "Failed open file";
            }
            else
            {
                file.seekg(0, ios_base::beg);
                while(file.read((&cl), sizeof(char))) // Поток возвращает 0, если ничто не считано, можно сунуть функцию в проверку цикла
                {
                    type = static_cast<int>(cl);
                    switch (type)
                    {
                    case 3:
                        new belt();
                        break;
                    
                    case 4:
                        new trousers();
                        break;
                    
                    case 5:
                        new table();
                        break;

                    case 6:
                        new chair();
                        break;
                    
                    default:
                        cout << "wtf type" << endl;
                        break;
                    };
                    file.read(reinterpret_cast<char*>(&obj_size), sizeof(int));
                    item::list_of_id[item::max_elem-1]->get(file);
                    item::list_of_id[item::max_elem-1]->id = newid;
                    newid++;
                }; 
            };
            file.close();
        };

void item::massive() // вывод содержимого массива в консоль
        {
            for (int i = 1; i < 200; i++)
            {
                if (list_of_id[i] != 0)
                {
                    cout << i << " ";
                    list_of_id[i]->get();    
                };
            };
        };

void item::clear() // очистка массива
        {
            for (int i = 1; i < 200; i++)
            {
                if (list_of_id[i] != 0)
                {
                    delete(list_of_id[i]);
                };
            };
        };

void item::rearrange() // сортировка массива объектов базового класса (кроме первого элемента (забронирован для корзины))
{
    item* sorted [200];
    for (int i = 0; i < 200; i++)
    {
        if (sorted[i] != 0)
        {
            sorted[i] = 0;
        };
    };
    int j = 0;
    for (int i = 1; i < 200; i++)
    {
        if (list_of_id[i] != 0)
        {
            sorted[j] = list_of_id[i];
            j++;
            list_of_id[i] = 0;
        };
    };
    for (int i = 0; i < 200; i++)
    {
        if (sorted[i] != 0)
        {
            list_of_id[i+1] = sorted[i];
        };
    };
       
};