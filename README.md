<img width="128" height="128" alt="logo" src="https://github.com/user-attachments/assets/d6b64974-ff15-4618-9768-eb550f4debb1" />

# Wordle-Solver

## Desktop Wordle Solver Application featuring:
 ### Word Scoring System
  * Words are given a score based on the values of the letters they contain, based on remaining words in the word list.
 ### Adaptive Elimination
  * Application recalculates words with every run.
 ### Automatic Dictionary Switching
  * Application will automatically swap to a larger word list when the core word list runs out of possible answers. This will solve words like "pshaw" without harming optimization.
  * Users are also able to manually swap between word lists if desired.
 ### Low System Requirements
  * Native GTK4 Application written in C and C++.
  * ≈200mb of memory tested (Windows).
  * ≈74mb of memory tested (Linux).
  * **Runs fully offline.**

## Supported Operating Systems
* ### Official Releases (x86-64)
  * [Linux (Debian 13) release](https://github.com/max-kaiser-soft/Wordle-Solver/releases) (works on some other Linux distributions like Fedora 44)
  * [Windows (64 bit) release](https://github.com/max-kaiser-soft/Wordle-Solver/releases)
* ### Other Platforms
  * Users are free to compile source code for other systems.

## Running Application
  ### Linux
  * Extract tar.gz file and execute "Wordle Solver" Linux binary within the
 same directory.
  * Do not remove other files.
  * Linux Release is compiled for Debian 13, but it successfully ran on some other distributions (results may vary).
 ### Windows
  * Extract zip file and execute "Wordle Solver.exe" within the same folder.
  * Do not remove other files.
  * Application is compiled for 64 bit Windows.

## Tutorial
  This Wordle Solver looks little like an actual Wordle game. This design choice was deliberate, as copying the style of the actual game limits what users can do with a solver. 
  
  Application allows users to input for as many guesses as they want, which can surpass a standard Wordle game. This gives users more freedom in how they go through different words.

  ### There are 7 main sections of the application:
  
   #### Final letters section
  <img width="868" height="101" alt="image" src="https://github.com/user-attachments/assets/36a0df8d-4811-4579-aeba-736b03389314" />
  <blockquote>
    
   This section is for final letters. The "green letters" in the game.

   These text boxes are in order of appearance in the word.

   Only one letter is allowed in each textbox.
   
  </blockquote>
  
   #### Unordered letters section
   <img width="866" height="93" alt="image" src="https://github.com/user-attachments/assets/0dc92d4a-a724-46a0-be15-fcf62a91b03b" />
  <blockquote>
    
   This section is for unordered but known letters, the "yellow letters" in the game.

   These text boxes are in order of appearance in the word.

   The user can input as many letters per textbox as they would like.
    
  </blockquote>
  
   #### Wrong letters section
   <img width="353" height="84" alt="image" src="https://github.com/user-attachments/assets/81222e4d-ffc3-4e88-87b4-8927d7f32fda" />
  <blockquote>
    
   This section is for wrong letters, the "gray letters" in the game.

   The user can input as many letters in the textbox as they would like.

   As order does not matter, there is just one text box.
    
  </blockquote>

---

  #### Note

   Spaces, commas or other symbols are not needed for these text boxes, but the application will know to skip them if they are inputted.
   
   Users are free to input them for visual clarity.
   
   Lowercase and capital letter (or a mix of the two) are allowed.


  This input:
  
   <img width="351" height="83" alt="image" src="https://github.com/user-attachments/assets/eb07c9f4-6c69-4b1f-af33-2849552c472a" />
    
   Will be interpreted as:
    
<img width="352" height="79" alt="image" src="https://github.com/user-attachments/assets/0f98fcc6-6b37-469f-b66a-bfb1791cc42d" />

---
  
   #### "Get Words" button
   <img width="345" height="49" alt="image" src="https://github.com/user-attachments/assets/709dbc06-c99e-41a7-848e-4ecf6d969cdc" />
  <blockquote>
    
   This button will read the relevant word list and calculate the ideal words to makes as a guess based on user inputted letters (or lack of letters).
   

   A score is given to each word that is based on how good of a guess it is.
   
   
   Some words may have the same score, this means that they are equally good guesses at that moment. This is especially obvious in anagrams, like "later" and "alter".
    
  </blockquote>
   
   #### "Restart" button
   <img width="176" height="46" alt="image" src="https://github.com/user-attachments/assets/a6d98727-5b9b-44f5-a151-486c951229bc" />
  <blockquote>
   This button resets the application. It clears all user inputs and resets the word list. 
    
  </blockquote>

   #### Results window
   <img width="117" height="298" alt="image" src="https://github.com/user-attachments/assets/09e666a4-08e6-481e-9b9c-452fb1dfbda9" />
  <blockquote>
    
   This window will display the top ten (or less) words to guess next.
   
   
   If there are no more possible words to display, it will print "No words found." In this case, it is possible that there are no possible words in the word lists or there was a user input mistake that removed all words (like putting the same letter in both the Final letters section and the Wrong letters section). Be sure to double check inputs when this happens.
    
  </blockquote>

   #### Word list drop down
   <img width="358" height="45" alt="image" src="https://github.com/user-attachments/assets/1b24651f-d422-44c7-9a7b-da46260488f0" />
  <blockquote>
    
   This allows for the user to specify which word list they would like to use.
   
    
   It is recommended to use the "Automatic" mode, as it swaps to the larger list when the smaller one runs out of words.
  </blockquote>
  
  







## Explanation




