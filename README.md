A search engine implemented in c using a word-level inverted index (Trie).

# Compile and run
A Makefile is provided. (-g is default).

Run the program with 
                  ```./minisearch -i [inputfile.txt] -k [num of results to return for each querry]```
                  or
                  ```./minisearch -r [inputfile.txt] ```     (K is 10 by default).
               
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
