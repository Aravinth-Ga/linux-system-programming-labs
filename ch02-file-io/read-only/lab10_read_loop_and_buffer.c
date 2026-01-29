/*
    lab10 : This program demonstrates reading from a file using a loop and a buffer.

    System Calls covered :
        open()
        read()
        write()
        close()

    Flags covered :
        O_RDONLY
    
    What this lab teaches :
        1. How to read data from a file in chunks using a buffer.
        2. How to handle end-of-file and read errors properly.
        3. How to keep track of the total number of bytes read.

    Error handled :
        1. Error in open()
        2. Error in read()
        3. Error in write()
        4. Error in close()
*/