// #include <avr/io.h>
// #include <stdlib.h>
// #include <util/delay.h>

// #define N 36

// void error(){
//   while(1){
//     PORTB ^= 1 << 7;
//     _delay_ms(100);
//     PORTB ^= 1 << 7;
//     _delay_ms(100);
//   }
// }

// void ok(){
//   while(1){
//     PORTB ^= 1 << 7;
//     _delay_ms(1000);
//     PORTB ^= 1 << 7;
//     _delay_ms(1000);
//   }
// }

// int main(void) {
//   // make the LED pin an output for PORTB5
//   DDRB = 1 << 7;
//   while (1) {

//     int8_t i,j; // 2 bytes
//     int* A,*B,*C; // 6 bytes
//     A = (int*)(malloc(N * N * sizeof(int))); //n*n*2 bytes
//     B = (int*)(malloc(N * N * sizeof(int))); //n*n*2 bytes
//     C = (int*)(malloc(N * N * sizeof(int))); //n*n*2 bytes
   
//     if (A==NULL || B==NULL || C==NULL) error();

//     ok();
//   }

//   return 0;
// }