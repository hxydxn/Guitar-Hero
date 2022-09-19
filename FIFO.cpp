// FIFO.cpp
// Runs on any LM3Sxxx
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Daniel Valvano
// 1/17/2020

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to the Arm Cortex M",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2020

 Copyright 2020 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
#include <stdint.h>

#include "FIFO.h"
#include "ST7735.h"


// A class named Queue that defines a FIFO
Queue::Queue(){
  // Constructor - set PutI and GetI as 0. 
  // We are assuming that for an empty Queue, both PutI and GetI will be equal
  // lab 9
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
    // lab 9
  return false; 
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
    // lab 9
  return false; 
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
    // lab 9
  return false; 
}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
    // lab 9
  return false; 
}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
    // Finding number of elements in queue  
    // lab 9
  return ; 
}


