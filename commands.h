const int kMaxSizeOfMessage = 30;
const int INBOX = 5;
TFileIOResult nBTCmdRdErrorStatus;
int nSizeOfMessage;
ubyte nRcvBuffer[kMaxSizeOfMessage];

/*! \brief Check if commands are given by the app.
 *
 *	\param[in] int *next_crossroad The action that has to be executed on the next crossroad.
 */
void check_bluetooth(Queue *next_crossroad_queue) {
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
            displayCenteredTextLine(3,"GO: UP");
            enqueue(next_crossroad_queue, 3);
            status = 1;
        }
        else if (s == "LEFT"){
            displayCenteredTextLine(3, "GO: LEFT");
            enqueue(next_crossroad_queue, 1);
        }
        else if (s == "RIGHT"){
            displayCenteredTextLine(3, "GO: RIGHT");
            enqueue(next_crossroad_queue, 2);
        }
        else if (s == "DOWN"){
            brake(100);
        }
        else if (s == "FIRE"){
            displayCenteredTextLine(3, "TURBO: ACTIVATED");
        	speed +=1;
        }
        displayCenteredTextLine(4, s);
    }
}
