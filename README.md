## TCP-IP-Banking-Console-Application

**Project Name:** Programming of Money Transfer Simulation Over Accounts Over TCP Socket

**Project Topic:** Two console applications were developed as TCP Server and TCP Client. Transactions on the user accounts, cards and balances of the accounts (database.txt) belonging to different banks previously created in the server are performed on the server side.It is possible to transfer money from the client via the TCP server. Users can log in with their user name and password in the Client application, view their balance, and send money to the specified user by typing the other customer number registered in the system, name-surname and the amount to be sent, if there is enough balance.If there is not enough balance, the transaction cannot be performed. Also, the user can withdraw and deposit money from his own balance. When making a transfer from one account to another, if the transferred account belongs to a different bank, a predetermined amount is deducted.

   **Application is created on Linux so you can not clearly run on Windows, you have to modify somethings.**

**RUN:** 

*tcpServer.out and tcpClient.out are debugged files and ready to run in its directory with commands:*
```
./tcpServer.out
./tcpClient.out
```

If it doesn't work, you can compile again from tcpServer.cpp and tcpClient.cpp files.

All infos about users are saved in _database.txt_ in order _username_, _name_, _surname_, _password_, _bankName_, _accountNo_, _amount._

**You can access a video how to use this app from this [Youtube Link](https://www.youtube.com/watch?v=h0GYwevohPU&t)**



