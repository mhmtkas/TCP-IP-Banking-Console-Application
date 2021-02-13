#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <fstream>
#include <algorithm>
 
using namespace std;

//Mehmet Kas

struct Bank{
    
    string name;
    int hesapNo;
    int totalAmount;

};
struct Person{
    string userName;
    string name;
    string surName;
    string password;
    Bank banka;

};
static void addValuesToTxt();
Person *dataBase()
{       
        static Person musteri1;
        static Person musteri2;
        static Person musteri3;
        static Person musteri4;
        static Person musteri5;
        static Person musteri6;

        static Person arrayPerson[6] = {musteri1,musteri2,musteri3 ,musteri4,musteri5,musteri6};
    return arrayPerson;
}  
string *returnArray(Person person){
    static string array[5];

    
    array[0] = person.name;
    array[1] = person.surName;
    array[2] = person.banka.name;
    array[3] = to_string(person.banka.hesapNo);
    array[4] = to_string(person.banka.totalAmount);
    return array;
    }
static int userNameCheck(string name)
{
    Person * arrayPerson;
    arrayPerson = dataBase();
    int tmp =0;

    string isim;
    for(int i=0;i<6;i++){
        isim = (arrayPerson+i)->userName;
        if((name) == (isim)){
            return 1;
        }
    }
    cout << "Wrong Username" <<endl;
    return 0;
}
static int userPassCheck(string pass,string name)
{
    Person * arrayPerson;
    arrayPerson = dataBase();
    int tmp =0;
    
    //cout << arrayPerson->password << endl;
    //cout << (arrayPerson+1)->nameSurname <<endl;
    string password;
    string isim;
    for(int i=0;i<6;i++){
        isim = (arrayPerson+i)->userName;
        if((name) == (isim)){
            password = (arrayPerson+i)->password;
            
            if((pass) == (password)){
                cout << "Correct Password" <<endl;
                cout << "Login Process Succesfull."<< endl;
                return 1;
            }
        }
    }
    cout << "Wrong Password" <<endl;
    cout << "Login Failed."<< endl;
    return 0;
}
static string *showAccountInfo(string userName){
    Person *p;

    p=dataBase();

    string isim,*array;

    for(int i=0;i<6;i++){

        isim = (p+i)->userName;

        if((userName) == (isim)){

            array = returnArray(*(p+i));
            
            return array;
            
        }
    }




    return 0;
}
static int deposit(string username,int value){

    Person *p;


    p=dataBase();
    string isim;

    for(int i=0;i<6;i++){

        isim = (p+i)->userName;

        if((username) == (isim)){

            (p+i)->banka.totalAmount += value;
            
            return (p+i)->banka.totalAmount;
            
        }
    }




    return 0;
}
static int withdraw(string username,int value){
     Person *p;
    p=dataBase();
    string isim;

    for(int i=0;i<6;i++){

        isim = (p+i)->userName;

        if((username) == (isim)){
            if((p+i)->banka.totalAmount>=value){

                (p+i)->banka.totalAmount -= value;
            
                return (p+i)->banka.totalAmount;
            }
            else{
                return -1;
            }
            
        }
    }
    return 0;
}
static int transfer(string username,int accountNo,string tarName,string tarSurname, int value){
    Person *p;
    p=dataBase();
    string isim,gidenIsim,gidenSoyisim;
    int userValue,bankaNo,gidenValue, islemUcreti = 0;
    int locationUser,locationTar=-1;

    transform(tarName.begin(), tarName.end(),tarName.begin(), ::toupper);
    transform(tarSurname.begin(), tarSurname.end(),tarSurname.begin(), ::toupper);
    
    
    

    for(int i=0;i<6;i++){ //find the user's total value 
        isim = (p+i)->userName;
        if((username) == (isim)){
            locationUser = i;
            userValue = (p+i)->banka.totalAmount;
            break;
        }
    }
    for(int i=0;i<6;i++){
        bankaNo = (p+i)->banka.hesapNo;
        
        gidenIsim = (p+i)->name;
        
        gidenSoyisim = (p+i)->surName;

        transform(gidenIsim.begin(), gidenIsim.end(),gidenIsim.begin(), ::toupper);
        transform(gidenSoyisim.begin(), gidenSoyisim.end(),gidenSoyisim.begin(), ::toupper);
        

        if((accountNo == bankaNo) && (gidenIsim==tarName) && (gidenSoyisim==tarSurname)){
            locationTar = i;
            break;
        }
    }
    if(locationTar==-1){
        return -1;
    }
    if((p+locationUser)->banka.name!=(p+locationTar)->banka.name)
        {
        islemUcreti = 5;
        } 
    
    if(userValue >= (value + islemUcreti))
    {
        if (islemUcreti>0)
        {
            (p+locationUser)->banka.totalAmount = (p+locationUser)->banka.totalAmount - (value+islemUcreti);
            (p+locationTar)->banka.totalAmount = (p+locationTar)->banka.totalAmount + value;
            return (p+locationUser)->banka.totalAmount;
        }

        (p+locationUser)->banka.totalAmount = (p+locationUser)->banka.totalAmount - value;
        (p+locationTar)->banka.totalAmount = (p+locationTar)->banka.totalAmount + value;
        return (p+locationUser)->banka.totalAmount;

    }else
    {

        return -2;
    }



}
int socket()
{
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return -1;
    }
 
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);
 
    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);
    cout << "Server is waiting.." << endl;
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
 
    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " connected on port " << ntohs(client.sin_port) << endl;
    }
 
    // Close listening socket
    close(listening);
 
    // While loop: accept and echo message back to client
    char buf[4096];
    string userName,userPass;
    
    while (true)
    {
        memset(buf, 0, 4096);
 
        // Wait for client to send data
        

        int bytesReceivedName = recv(clientSocket, buf, 4096, 0);

        if (bytesReceivedName == -1)
        {
            addValuesToTxt();
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }
 
        if (bytesReceivedName == 0)
        {   
            addValuesToTxt();
            cout << "Client disconnected " << endl;
            break;
        }

        string error = "error";
        string correct = "correct";
        string name ;

        cout << "Trying to Login: " << string(buf, 0, bytesReceivedName) << endl;
        
        if(true)
        {   
            name = (string(buf, 0, bytesReceivedName));

            if(userNameCheck(name)){
                cout << "correct username" << endl;
            }

            send(clientSocket, correct.c_str(), correct.size() + 1, 0);
            int bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

            if(userPassCheck((string(buf, 0, bytesReceivedPass)),name)){
                string choice;
                send(clientSocket, correct.c_str(), correct.size() + 1, 0);
                cout << "Waiting for choice."<<endl;
                cout << " " << endl;
                bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                if (bytesReceivedPass == -1)
                {
                    addValuesToTxt();
                    cerr << "Error in recv(). Quitting" << endl;
                    break;
                }
 
                if (bytesReceivedPass == 0)
                {   
                    addValuesToTxt();
                    cout << "Client disconnected " << endl;
                    break;
                }


                choice = (string(buf, 0, bytesReceivedPass));
                cout << choice <<endl;
                cout << " " << endl;
                int choiceInt= stoi(choice);
                string response = "1";
                string *array;
                array = showAccountInfo(name);
                switch (choiceInt){
                    case 1:{
                        cout << "***Showing Account Info***"<<endl;
                        cout << " " << endl;
                        for(int i=0;i<5;i++){       
                            send(clientSocket, (array[i]).c_str(), (array[i]).size() + 1, 0);  
                            bytesReceivedPass = recv(clientSocket, buf, 4096, 0);  
                        }
                    }
                    break;

                    case 2:{

                        cout << "***Starting Deposit Process***"<<endl;
                        cout << " " << endl;
                        
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        
                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int totalValue = deposit(name,value);
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;  

                    case 3:{
                        cout << "***Starting Withdraw Process***"<<endl;
                        cout << " " << endl;
    
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }
                        
                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int totalValue = withdraw(name,value); 
                        string valueStr = to_string(totalValue);
                        
                        send(clientSocket, valueStr.c_str(), valueStr.size() + 1, 0);

                    }
                    break;

                    case 4:
                    {
                        cout << "***Starting Transfer Process***"<<endl;
                        cout << " " << endl;
                        send(clientSocket, array[4].c_str(), array[4].size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);

                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }
                        
                        int targetNo = stoi(string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tarName = (string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        string tarSurname = (string(buf, 0, bytesReceivedPass));
                        
                        send(clientSocket, response.c_str(), response.size() + 1, 0);
                        bytesReceivedPass = recv(clientSocket, buf, 4096, 0);
                        
                        if (bytesReceivedPass == -1)
                        {
                            addValuesToTxt();
                            cerr << "Error in recv(). Quitting" << endl;
                            break;
                        }
 
                        if (bytesReceivedPass == 0)
                        {   
                            addValuesToTxt();
                            cout << "Client disconnected " << endl;
                            break;
                        }

                        int value = stoi(string(buf, 0, bytesReceivedPass));
                        int snc = transfer(name,targetNo,tarName,tarSurname,value);
                        string sncStr = to_string(snc);

                        send(clientSocket, sncStr.c_str(), sncStr.size() + 1, 0);
                    }    
                    break;         
                }
            }
            else{
                send(clientSocket, error.c_str(), error.size() + 1, 0);
            }
        }
        else{
            send(clientSocket, error.c_str(), error.size() + 1, 0);
        }       
    }
 
    // Close the socket
    close(clientSocket);
 
    return 0;
}
static void addValuesFromTxt(){

    Person * arrayPerson;
    arrayPerson = dataBase();

    string userName,name,surname,password,bankaName;
    int hesapNo,paraValue;
    
    fstream dosyaOku("database.txt");
    string satir = "";

    if ( dosyaOku.is_open() ){
        
        for(int i=0;i<6;i++){

            dosyaOku >> userName >> name>> surname >> password >> bankaName >> hesapNo >> paraValue; 
            
            (arrayPerson+i)->userName = userName;
            (arrayPerson+i)->name = name;
            (arrayPerson+i)->surName = surname;
            (arrayPerson+i)->password = password;
            (arrayPerson+i)->banka.name = bankaName;
            (arrayPerson+i)->banka.hesapNo = hesapNo;
            (arrayPerson+i)->banka.totalAmount = paraValue;
        }
        dosyaOku.close();
    }
    
}
static void addValuesToTxt(){
    Person * arrayPerson;
    arrayPerson = dataBase();

    string userName,name,surname,password,bankaName;
    int hesapNo,paraValue;

    fstream dosyaOku("database.txt",ios::out | ios::ate);
    string satir = "";

    if ( dosyaOku.is_open() ){
        
        for(int i=0;i<6;i++){
            userName = (arrayPerson+i)->userName ;
            name = (arrayPerson+i)->name;
            surname = (arrayPerson+i)->surName;
            password = (arrayPerson+i)->password;
            bankaName = (arrayPerson+i)->banka.name;
            hesapNo = (arrayPerson+i)->banka.hesapNo;
            paraValue = (arrayPerson+i)->banka.totalAmount;

            dosyaOku << userName << " "<< name << " " << surname << " "  << password << " "  << bankaName << " "  << hesapNo << " "  << paraValue << endl; 
        }
        dosyaOku.close();
    }



}
int main(int argc, char* argv[]){

    
    addValuesFromTxt();
    socket();

    

    return 0;
}
 
