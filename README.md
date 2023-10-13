# Unscrabble
A straightforward TUI (Linux) game based on the real Scrabble that challenges you to compose the best word given a set of letters. Each word's value and repartition is calculated via 
the official Scrabble rules.

### How to install and play
1. Git clone the repos where you want it to be:
 ```bash
sudo git clone https://github.com/Nirij3m/Unscrabble.git
```
2. Open your terminal and go to the freshly cloned repos
 ```bash
cd ~/Unscrabble
```
3. Give to the binary file the perms to execute
```bash
sudo chmod +x ./unscrabble
  ```
4. Run the game !
```bash
 ./unscrabble
  ```
  tips: You can at all time CTRL+C to exit the program.

### Compile from source
It'e very likely that you won't be able to run the binary file directly, mainly because the .c file was compiled with a different version of gcc than yours. In that case,
you still can compile it from source!
- First install the ncurses library
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```
- Then compile the main.c file
```bash
sudo gcc main.c src/nodeTrie.c src/scrabble.c -lncurses -o unscrabble
```
You can now execute the binary file (refer to the bullet points above this section).


