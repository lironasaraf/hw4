#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


#define NUM_LETTERS		26
#define MAX_STR_LEN		1048

typedef enum
{
	FALSE = 0,
	TRUE=1
} boolean;


typedef struct TrieNode
{
	char				letter;					// The letter of this node
	unsigned int		count;					// Count of words that end with this node letter
	struct TrieNode*	children[NUM_LETTERS];	// The node children array
	int					childrenIndex;			// The node children array iterator
	struct TrieNode*	parent;					// The parent of this node

} TrieNode;


// The root of the trie tree
TrieNode *rootNode = NULL;





// This function creates a new TRIE node for a specific letter
TrieNode *createNode(TrieNode *parent, char letter)
{
	int i;

	// Create new node
	TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode)); // Zion TBD

	// Initialize the new node
	node->parent = parent;
	node->letter = letter;
	node->count = 0;
	for (i = 0;  i < NUM_LETTERS; i++)
		node->children[i] = NULL;
	
	return node;
}


// This function calculates and returns the index of a letter in children[] array
int letterIndex(char letter)
{
	return (letter - 'a');
}


// This function adds a word to the TRIE
void addWord(char *word)
{
	int		i, index;
	char	letter;

	TrieNode *node = rootNode;

	for (i = 0; i < (int)strlen(word); i++)
	{
		letter = word[i];

		// Find the letter index
		index = letterIndex(letter);

		// Create child node if not exist, with current node as a parent
		if (node->children[index] == NULL)
		{
			node->children[index] = createNode(node, letter);
		}

		// Move to child node
		node = node->children[index];

		// For last letter (end of word) increase the word counter by one
		if (i == (int)strlen(word) - 1)
			node->count++;
	}
}


// This function prints all words in the TRIE in ascending order
void printAscendingOrder()
{
	TrieNode	*node = rootNode;
	char		word[MAX_STR_LEN];
	int			wordIndex = 0;;
	
	// Reset node children iterator
	node->childrenIndex = 0;

	while (1)
	{
		if (node->childrenIndex < NUM_LETTERS)
		{
			// Get current child and decrease node children iterator
			TrieNode *child = node->children[node->childrenIndex++];

			// Check if current child exist
			if (child != NULL)
			{
				// Reset node children iterator
				child->childrenIndex = 0;

				// Add the letter to the word buffer
				word[wordIndex++] = child->letter;
				
				// If count > 0 than it is the end of our word, print it
				if (child->count > 0)
				{
					// Add end-of-string to our word and print it
					word[wordIndex] = '\0';
					printf("%s %d \n", word, child->count);
				}

				// Continue with the child node
				node = child;
			}
		}
		else // All children were scaned, move to the parent
		{
			// Go one pos back in the word buffer
			wordIndex--;

			// Go to parent
			if (node->parent == NULL) // Done, we reached to root's parent (only root's parent is NULL)
				break;

			node = node->parent;
		}
	}
}



// This function prints all words in the TRIE in descending order
void printDescendingOrder()
{
	TrieNode	*node = rootNode;
	char		word[MAX_STR_LEN];
	int			wordIndex = 0;;
	
	// Reset node children iterator
	node->childrenIndex = NUM_LETTERS - 1;

	while (1)
	{
		if (node->childrenIndex >= 0) 
		{
			// Get current child and decrease node children iterator
			TrieNode *child = node->children[node->childrenIndex--];

			// Check if current child exist
			if (child != NULL)
			{
				// Reset node children iterator
				child->childrenIndex = NUM_LETTERS - 1;

				// Add the letter to the word buffer
				word[wordIndex++] = child->letter;
				
				// If count > 0 than it is the end of our word, print it
				if (child->count > 0)
				{
					// Add end-of-string to our word and print it
					word[wordIndex] = '\0';
					printf("%s %d \n", word, child->count);
				}

				// Continue with the child node
				node = child;
			}
		}
		else // All children were scaned, move to the parent
		{
			// Go one pos back in the word buffer
			wordIndex--;

			// Go to parent
			if (node->parent == NULL) // Done, we reached to root's parent (only root's parent is NULL)
				break;

			node = node->parent;
		}
	}
}


// This function deletes the whole TRIE
void deleteTrie()
{
	TrieNode *node = rootNode;
	
	// Reset node children iterator
	node->childrenIndex = 0;

	while (1)
	{
		if (node->childrenIndex < NUM_LETTERS)
		{
			// Get current child and decrease node children iterator
			TrieNode *child = node->children[node->childrenIndex++];

			// Check if current child exist
			if (child != NULL)
			{
				// Reset node children iterator
				child->childrenIndex = 0;

				// Continue with the child node
				node = child;
			}
		}
		else // All children were scaned, move to the parent
		{
			TrieNode *parent = node->parent;
			
			// delete this node
			//printf("Deleting node %c\n", node->letter);				
			free(node);

			// Go to parent
			if (parent == NULL) // Done, we reached to root's parent (only root's parent is NULL)
				break;

			node = parent;
		}
	}
}


int main(int argc, char* argv[])
{
	int i, index;
	char str[MAX_STR_LEN];
	char word[MAX_STR_LEN];


	// Create root node with with NULL parent and letter (because the root has no parent and does not represent any specific letter)
	rootNode = createNode(NULL, '.');

	// Read input string
	//printf("Enter string please:\n");
	fgets(str, sizeof(str), stdin);


	// Cut input string to words
	index = 0;
	for (i = 0; i < (int)strlen(str); i++)
	{
		// Change letter to lower case
		char letter = tolower(str[i]);

		if (letter >= 'a' && letter <= 'z')
		{
			// Printable letter add to word
			word[index++] = letter;
		}
		else if (letter == ' ') // If end of word
		{
			// Add end of word
			word[index++] = '\0';

			// Add the word to TRIE
			addWord(word);

			// Reset index to 0 to input the next word in the string
			index = 0;
		}
	}

	
	if ((argc > 1) && (strcmp(argv[1], "r") == 0))
	{
		// printf("\n\n===== Descending Order =====\n");
		printf("\n");
		printDescendingOrder();
	}
	else
	{
		// printf("\n\n===== Ascending Order =====\n");
		printf("\n");
		printAscendingOrder();
	}

	deleteTrie();

	return 0;
}
