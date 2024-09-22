#include <nds.h>
#include <stdio.h>

volatile int frame = 0;
char turn, winner;
char board[3][3];
int turns;

void Vblank() {
	frame++;
}

void resetBoard(void)
{
	turn='X';
	winner='w';
	turns=1;
	for(int i=0; i<3; i++)
		for(int j=0; j<3; j++)
			board[i][j]=' ';
}

int main(void) {

	touchPosition touchXY;
	irqSet(IRQ_VBLANK, Vblank);
	consoleDemoInit();
	
	resetBoard();
	while(1) {
		iprintf("\x1b[0;0H");
		if(winner=='T')
			iprintf("Tie!\n");
		else if(winner!='w')
			iprintf("%c won!\n", winner);
		else
			iprintf("      \n");
		for(int i=0; i<3; i++)
		{
			iprintf("\n-------\n");
			for(int j=0; j<3; j++)
				iprintf("|%c", board[i][j]);
			iprintf("|");
		}
		iprintf("\n-------\n");
		
        scanKeys(); // Scan current key/touch input
        int keys = keysDown(); // Check which keys are held

        if (keys & KEY_TOUCH && winner == 'w') {  // If touch screen is being pressed
            touchRead(&touchXY); // Get touch data
            //x: 8-20, 24-36, 40-52, y: 16-28, 32-44, 48-60
            int r=(touchXY.py-16)/16;
            if(r>=0&&r<3)
            {
            	int c=(touchXY.px-8)/16;
            	if(c>=0&&c<3&&board[r][c]==' ')
            	{
            		board[r][c]=turn;
					if((board[0][c]==turn&&board[1][c]==turn&&board[2][c]==turn)||(board[r][0]==turn&&board[r][1]==turn&&board[r][2]==turn)||(board[1][1]==turn&&
						((board[0][0]==turn&&board[2][2]==turn)||(board[0][2]==turn&&board[2][0]==turn))))
						winner=turn;
					else if(turns++==9)
						winner='T';
					else
						turn=turn=='X'?'O':'X';
            	}
            }
        }
        if (keys & KEY_START && winner != 'w')
        	resetBoard();

        swiWaitForVBlank(); // Wait for next frame (VBlank)
    }


	return 0;
}
