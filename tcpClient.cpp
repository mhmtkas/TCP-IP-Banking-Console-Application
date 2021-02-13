#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

using namespace std;



int client()
{
    //	Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        return 1;
    }

    //	Create a hint structure for the server we're connecting with
    int port = 54000;
    string ipAddress = "127.0.0.1";

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //	Connect to the server on the socket
    int connectRes = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connectRes == -1)
    {
        return 1;
    }

    //	While loop:
    char buf[4096];
    string user;
	int i=0;
    do {

		if(i==0){
			cout << "Username: ";
			cin >> user;
		}

        //		Send to server
        int sendRes = send(sock, user.c_str(), user.size() + 1, 0);
        
		if (sendRes == -1 )
        {
            cout << "Could not send to server! Whoops!\r\n";
            continue;
        }

        //		Wait for response
        
		memset(buf, 0, 4096);
        string gelen;
        int bytesReceived = recv(sock, buf, 4096, 0);

        int choice;
        string choiceStr;
        int i=1;
        
        //username checking
        if (true){
            
			cout << "Password: ";
			cin >> user;
			send(sock, user.c_str(), user.size() + 1, 0);
            bytesReceived = recv(sock, buf, 4096, 0);
            //password checking
            if (string(buf,0, bytesReceived) == ("correct")){
                cout<< ""<< endl;
                cout << "--Giriş İşlemi Başarılı--" << endl;
                cout<< ""<< endl;
                do {

                cout << "1-Hesap Bilgilerini Gösterme." << endl;
                cout << "2-Hesaba Para Yatırma." << endl;
                cout << "3-Hesaptan Para Çekme." << endl;
                cout << "4-Başka Hesaba Para Yatırma." << endl;
                cout<< ""<< endl;
                cout << "Lütfen yapmak istediğiniz işlem numarasını giriniz: ";
                cin>>choice;
                cout<< ""<< endl;
                if(choice<=0 || choice>=5) {
                    cout << "Yanlış bir seçim yapıldı.Yeniden Deneyin."<<endl;
                    cout<< ""<< endl;
                }

                }while(choice<=0 || choice>=5);

                
                string response="1";
                switch(choice) {
                    case 1:{
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        for(int i=0;i<5;i++){
                            if(i==0){
                                cout << "İsim: ";
                            }
                            else if(i==1){
                                cout << "Soyisim: ";
                            }
                            else if(i==2){
                                cout << "Banka Adı: ";
                            }
                            else if(i==3){
                                cout << "Hesap Numarası: ";
                            }else
                            {
                                cout << "Hesap Bakiyesi: $";
                                
                            }
                            bytesReceived = recv(sock, buf, 4096, 0);
                            cout << string(buf,0, bytesReceived) << endl;
                            send(sock, response.c_str(), response.size() + 1, 0);

                        }
                        cout<< " "<< endl;
                }
                    break;

                    case 2:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        cout << "Hesap Bakiyeniz: "<< (string(buf,0, bytesReceived)) << endl;
                        cout<< ""<< endl;
                        
                        cout << "Lütfen yatırmak istediğiniz tutarı giriniz: ";
                        cin >> value;
                        cout<< ""<< endl;
                        string valueStr;
                        valueStr= to_string(value);
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        cout << "Hesap Bakiyeniz: " << (string(buf,0, bytesReceived)) <<endl;
                        cout<< ""<< endl;

                    }
                    break;

                    case 3:{
                        int value;
                        choiceStr=to_string(choice);
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        string oldValue = (string(buf,0, bytesReceived));
                        int toplamBakiye = stoi(oldValue);
                        do {
                            if(value > toplamBakiye){
                            cout << "Hesabınızda bulunandan daha fazla bir miktar girdiniz!" << endl;
                            }
                        cout << "Hesap Bakiyeniz: "<< oldValue << endl;
                        cout<< ""<< endl;
                        cout << "Lütfen çekmek istediğiniz tutarı giriniz: ";
                        cin >> value;
                        cout<< "" << endl;
                        }while(value > toplamBakiye);
                        string valueStr;
                        valueStr= to_string(value);

                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        
                        toplamBakiye = stoi((string(buf,0, bytesReceived)));
                        if(toplamBakiye != -1) {
                            cout << "Hesap Bakiyeniz: " << (string(buf,0, bytesReceived)) <<endl;
                        }
                        cout<< ""<< endl;
                    }
                    break;

                    case 4:
                    {
                        string tarName,tarSurname,noStr,valueStr;
                        int targetNo,value;
                        choiceStr=to_string(choice);
                        
                        send(sock, choiceStr.c_str(), choiceStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        string oldValue = (string(buf,0, bytesReceived));
                        cout << "***Farklı banka ile yapılan işlemlerde $5 işlem ücreti olarak göndericiden alınır.***"<< endl;
                        cout<< "" << endl;
                        cout << "Hesap Bakiyeniz: $"<< oldValue << endl;
                        cout << "Para Gönderilecek Hesap No: ";
                        cin >> targetNo;
                        noStr = to_string(targetNo);
                        
                        send(sock, noStr.c_str(), noStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Para Gönderilecek Kişi İsmi: ";
                        cin >> tarName;
                        
                        send(sock, tarName.c_str(), tarName.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Para Gönderilecek Kişi Soyismi: ";
                        cin >> tarSurname;
                        
                        send(sock, tarSurname.c_str(), tarSurname.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);
                        
                        cout << "Göndermek istenen para miktarı: $";
                        cin >> value;
                        valueStr = to_string(value);
                        
                        send(sock, valueStr.c_str(), valueStr.size() + 1, 0);
                        bytesReceived = recv(sock, buf, 4096, 0);

                        string bakiyeLast = (string(buf,0, bytesReceived));
                        int bakiyeInt = stoi(bakiyeLast);
                        if(bakiyeInt==-1){
                            cout << "Yanlış hesap Bilgileri girildi."<<endl;
                            }
                        else if(bakiyeInt==-2)
                        {
                            cout << "Hesabınızda yeterli bakiye bulunmamaktadır."<<endl;
                        }
                        else{
                        
                             cout << "Hesap Bakiyeniz: $"<< bakiyeLast << endl;
                        }
                       

                    }
                    break;
                    
                    
                }

            }else{

                cout << "Kullanıcı Girişi Başarısız! Kullanıcı adı veya parola hatalı." << endl;
            
                continue;

            }

            
            
        }else
        {
            //cout << "Invalid Username" << endl;
            
            continue;
        }
        
    } while(true);

    //	Close the socket
    close(sock);

    return 0;
}




int main(){
	
	client();

}
