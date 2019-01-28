#include<iostream>
#include<iomanip>
#include<stdlib.h>
using namespace std;

struct patient
{
    string name;
    string desease;
    int age;
    struct patient *next;
};

typedef struct patient patient;

class PQ
{
    patient *newNode,*front,*rear;

    public:
        PQ()
        {
            front= new patient;
            rear= new patient;
            front->age=-1;
            rear->age=0;
            newNode= new patient;
            newNode->next=NULL;
            front->next=newNode;
            rear=newNode;
        }

        void accept();
        void delRec();
        bool isFull();
        bool isEmpty();
        void menu();
        void display();
};

void PQ::display()
{
    patient *dummy;

    if(!isEmpty())
    {
        cout<<"-------------------------------"<<endl;
        cout<<std::right<<std::setw(10)<<"Name"<<std::right<<std::setw(6)<<"Age"<<std::right<<std::setw(10)<<"Desease"<<endl;
        cout<<"-------------------------------"<<endl;
        dummy=front->next;
        do
        {
            cout<<std::right<<std::setw(10)<<dummy->name<<std::right<<std::setw(6)<<dummy->age<<std::right<<std::setw(10)<<dummy->desease<<endl;
            dummy=dummy->next;
        }while(dummy->next!=NULL);

        cout<<"-------------------------------"<<endl;
    }
    else
        cout<<std::right<<std::setw(20)<<"The queue is empty"<<endl;
}

bool PQ::isEmpty()
{
    if(rear==front->next)
        return true;
    return false;
}

void PQ::accept()
{



    cout<<"\nEnter Name :";
    cin>>rear->name;
    cout<<"Enter age :";
    cin>>rear->age;
    cout<<"Enter desease :";
    cin>>rear->desease;

    newNode=new patient;
    rear->next=newNode;
    rear=newNode;
    rear->next=NULL;

    display();

}

void PQ::delRec()
{
    patient *dummy;

    if(!isEmpty())
    {

       dummy=front;
       front=front->next;
       free(dummy);             //free is a member of stdlib.h
    }

    display();
}

void PQ::menu()
{
    int choice,ON=1;
    bool play=true;
    //arr *p=new arr;
    while(play)
    {
        cout<<"\nEnter your choice:\n1:Add a record\n2:Delete a record\n3:Check if queue is Empty\n4:Exit"<<endl;
        cin>>choice;
        switch(choice)
        {
            case 1:
                    while(ON)
                    {
                        accept();
                        cout<<"\nEnter 0 to exit accepting numbers :";
                        cin>>ON;
                    }

                    break;
            case 2:delRec();
                    cout<<"One record deleted"<<endl;
                    break;
            case 3:if(!isEmpty())
                        cout<<std::right<<std::setw(20)<<"The is queue NOT empty"<<endl;
                     else
                        cout<<"The Queque is empty"<<endl;
                    break;
            case 4:play=false;
                    break;
            default:cout<<"wrong choice entered";
        }
    }
}

int main()
{
    PQ serious, medium, general;

    int choice;
    bool play=true;

    while(play)
    {
        cout<<"\nEnter your choice:\n1:Serious\n2:Medium\n3:General\n4:Exit"<<endl;
        cin>>choice;
        switch(choice)
        {
            case 1:serious.menu();
                    break;
           case 2:medium.menu();
                    break;
            case 3:general.menu();
                    break;
            case 4:play=false;
                    break;
            default:cout<<"wrong choice entered";
        }
    }

    return 0;
}
