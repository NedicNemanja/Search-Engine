A search engine implemented in c using a word-level inverted index (Trie).

# Compile and run
A Makefile is provided. (-g is default).

Run the program with 

                  ./minisearch -i [inputfile.txt] -k [num of results to return for each querry]
                  
 or
                  
                 
                  ./minisearch -r [inputfile.txt]     
(K is 10 by default).
               
# Console options:

There are 4 basic commands which you can use:
  ```
  /search [Querry]
  ```
  To search the file with a querry
  ```
  /df
  ```
  To list all the words and the number of documents they appear in.
  or
  ```
  /df [word]
  ```
  To get the number of documents [word] apperars in.
  ```
  /tf [doc_id] [word]
  ```
  To get the number of times [word] appears in document with [doc-id].
  ```
  /exit
  ```
  To exit the console and terminate the program.
  
  # Examples
  ```
  /search one bitcoin network two
[two] not found.
  0.(   52)[+1.774553] one input and more than one output      
                       ^^^                     ^^^                                                                                                                            
  1.(   27)[+1.465023] Transactions consist of one or more inputs and one or more outputs When a      
                                               ^^^                    ^^^                                                                                                     
  2.(   66)[+1.331694] If the private key is lost the bitcoin network will not recognize any other    
                                                      ^^^^^^^ ^^^^^^^                                                                                                         
  3.(   25)[+1.244440] See also: Bitcoin network  
                                         ^^^^^^^                                                                                                                              
  4.(   33)[+1.098125] bitcoins to multiple recipients in one transaction As in a cash transaction          
                                                          ^^^                                                                                                                 
  5.(   68)[+1.021787] For example in 2013 one user claimed to have lost 7500 bitcoins worth $75 
                                           ^^^                                                                                                                                
  6.(   14)[+0.943000] amount each network node stores its own copy of the     
                                   ^^^^^^^                                                                                                                                    
  7.(   64)[+0.906407] digitally sign the transaction The network verifies the signature using the
                                                          ^^^^^^^                                                                                                             
  8.(    7)[+0.872548] the maintenance of the blockchain is performed by a network of communicating 
                                                                           ^^^^^^^                                                                                            
  9.(    9)[+0.841128] Y bitcoins to payee Z are broadcast to this network using readily available 
                                                                   ^^^^^^^  
```

```
/df transactions
transactions 9
```

```
/tf 37 transaction
2 transaction
```
