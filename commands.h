long nLastXmitTimeStamp = nPgmTime;
long nDeltaTime         = 0;
const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
TFileIOResult nBTCmdRdErrorStatus;
int nSizeOfMessage;
ubyte nRcvBuffer[kMaxSizeOfMessage];

void check_bluetooth(int *next_crossroad) {
    // Check to see if a message is available
    nSizeOfMessage = cCmdMessageGetSize(INBOX);
    if (nSizeOfMessage > kMaxSizeOfMessage)
        nSizeOfMessage = kMaxSizeOfMessage;
    if (nSizeOfMessage > 0){
        nBTCmdRdErrorStatus = cCmdMessageRead(nRcvBuffer, nSizeOfMessage, INBOX);
        nRcvBuffer[nSizeOfMessage] = '\0';
        string s = "";
        stringFromChars(s, (char *) nRcvBuffer);
        if (s == "UP"){
            displayCenteredTextLine(3,"We go up");
            *next_crossroad = 3;
            status = 1;
        }
        else if (s == "LEFT"){
            displayCenteredTextLine(3, "To the left it is");
            *next_crossroad = 1;
        }
        else if (s == "RIGHT"){
            displayCenteredTextLine(3, "Make a right turn");
            *next_crossroad = 2;
        }
        else if (s == "DOWN"){
            brake(100);
        }
        else if (s == "FIRE"){
        		speed +=1;
        }
        displayCenteredTextLine(4, s);
    }
}
