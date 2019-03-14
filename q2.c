#include <stdio.h>

int q2(int x, int y, int z){

    // int w = x - 10;
    switch(x){
        case 10:
            z += y;
            break;
        case 12:
            z = y - z;
            break;
        case 14:
            z = y > z;
            break;
        case 16:
            z = z > y;
            break;
        case 18:
            z = y == z;
        case 11:
        case 13:
		case 15:
		case 17:
        default:
            z = 0;
            break;
    }
    return z;
}