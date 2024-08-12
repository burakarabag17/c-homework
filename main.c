#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256
#define MAX_GENRES 100
#define MAX_DIRECTORS 100
#define INITIAL_CAPACITY 1000

struct Budget
{
    int budget;
    int year;
    char *title;
    char *titletype;
    char *top250;
};

struct Name
{
    char **genre;
    char *title;
    char **directors;
    char *mustsee;
    double rating;
    double score;
    char *url;
};

void displayMenu()
{
    printf("1. List of budget array\n");
    printf("2. List of name array\n");
    printf("3. List of genres\n");
    printf("4. List of the Movie Through the Years\n");
    printf("5. List of the Movie Through the Scores\n");
    printf("6. All Information of a Single Movie\n");
    printf("7. Frequency of the Genres\n");
    printf("8. Exit\n");
    printf("Select an option: ");
}

char *copy_and_tokenize(const char *token)
{
    // Allocate memory for the copy of the token
    char *token_copy = malloc(strlen(token) + 1);
    if (token_copy == NULL)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    // Copy the contents of token to token_copy
    strcpy(token_copy, token);

    // Tokenize the copied string
    char *genre_token = strtok(token_copy, ",");

    return genre_token;
}

// Budget array sıralama
int compareBudget(const void *a, const void *b)
{
    const struct Budget *budgetA = (const struct Budget *)a;
    const struct Budget *budgetB = (const struct Budget *)b;

    // Önce yıllara göre sıralama (azalan)
    if (budgetA->year != budgetB->year)
    {
        return budgetB->year - budgetA->year;
    }
    // Yıllar aynı ise bütçelere göre sıralama (azalan)
    if (budgetA->budget != budgetB->budget)
    {
        return budgetB->budget - budgetA->budget;
    }
    // Yıllar ve bütçeler aynı ise herhangi bir sıralama farkı yok
    return 0;
}

void cleanup(struct Budget *budgets, struct Name *names, int num_movies, FILE *file)
{
    if (file != NULL)
    {
        fclose(file);
    }
    for (int i = 0; i < num_movies; i++)
    {
        free(budgets[i].title);
        free(budgets[i].titletype);
        free(budgets[i].top250);

        if (names[i].genre != NULL)
        {
            for (int j = 0; j < MAX_GENRES && names[i].genre[j] != NULL; j++)
            {
                free(names[i].genre[j]);
            }
            free(names[i].genre);
        }

        free(names[i].title);

        if (names[i].directors != NULL)
        {
            for (int j = 0; j < MAX_DIRECTORS && names[i].directors[j] != NULL; j++)
            {
                free(names[i].directors[j]);
            }
            free(names[i].directors);
        }

        free(names[i].mustsee);
        free(names[i].url);
    }

    free(budgets);
    free(names);
}

void listBudgetArray(struct Budget *budgets, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Budget: %d, Year: %d, Title: %s, TitleType: %s, Top250: %s\n",
               budgets[i].budget, budgets[i].year, budgets[i].title, budgets[i].titletype, budgets[i].top250);
    }
}

void listNameArray(struct Name *names, int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("Genres: ");
        if (names[i].genre != NULL)
        {
            for (int j = 0; names[i].genre[j] != NULL; j++)
            {
                printf("%s", names[i].genre[j]);
                if (names[i].genre[j + 1] != NULL)
                {
                    printf(", ");
                }
            }
        }
        printf("\n");

        printf("Title: %s\n", names[i].title);

        printf("Directors: ");
        if (names[i].directors != NULL)
        {
            for (int j = 0; names[i].directors[j] != NULL; j++)
            {
                printf("%s", names[i].directors[j]);
                if (names[i].directors[j + 1] != NULL)
                {
                    printf(", ");
                }
            }
        }
        printf("\n");

        printf("Must See: %s\n", names[i].mustsee);
        printf("Rating: %.2f\n", names[i].rating);
        printf("Score: %.2f\n", names[i].score);
        printf("URL: %s\n\n", names[i].url);
    }
}

void listGenres(struct Name *names, int size)
{
#define MAX_GENRE_LENGTH 100
    char uniqueGenres[MAX_GENRES][MAX_GENRE_LENGTH];
    int genreCount = 0;

    for (int i = 0; i < size; i++)
    {
        if (names[i].genre != NULL)
        {
            for (int j = 0; names[i].genre[j] != NULL; j++)
            {
                int found = 0;
                for (int k = 0; k < genreCount; k++)
                {
                    if (strcmp(uniqueGenres[k], names[i].genre[j]) == 0)
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found && genreCount < MAX_GENRES)
                {
                    strncpy(uniqueGenres[genreCount], names[i].genre[j], MAX_GENRE_LENGTH - 1);
                    uniqueGenres[genreCount][MAX_GENRE_LENGTH - 1] = '\0'; // Güvenlik için sonlandırıcı ekliyoruz
                    genreCount++;
                }
            }
        }
    }
    printf("Genres:\n");
    for (int i = 0; i < genreCount; i++)
    {
        printf("%s\n", uniqueGenres[i]);
    }
}

int compareByYear(const void *a, const void *b)
{
    const struct Budget *movieA = (const struct Budget *)a;
    const struct Budget *movieB = (const struct Budget *)b;
    return movieB->year - movieA->year;
}

void listMoviesThroughYears(struct Budget *budgets, int size)
{
    struct Budget *sorted_movies = malloc(size * sizeof(*sorted_movies));

    if (sorted_movies == NULL)
    {
        perror("Memory allocation error!");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        sorted_movies[i] = budgets[i];
    }

    qsort(sorted_movies, size, sizeof(*sorted_movies), compareByYear);

    for (int i = 0; i < size; i++)
    {
        printf("%s, %d\n", sorted_movies[i].title, sorted_movies[i].year);
    }

    free(sorted_movies);
}

int compareByScore(const void *a, const void *b)
{
    const struct Name *movieA = (const struct Name *)a;
    const struct Name *movieB = (const struct Name *)b;

    // Puanları azalan sıraya göre sıralama
    if (movieB->score > movieA->score)
        return 1;
    if (movieB->score < movieA->score)
        return -1;
    return 0;
}

// Puanlara göre sıralı filmleri listeleme fonksiyonu
void listMoviesByScore(struct Name *names, int size)
{
    // names dizisini puanlara göre sıralıyoruz
    qsort(names, size, sizeof(struct Name), compareByScore);

    for (int i = 0; i < size; i++)
    {
        printf("%s, %.2f\n", names[i].title, names[i].score);
    }
}

void printMovieInfo(struct Name *names, int size)
{
    char searchTitle[MAX_LINE_LENGTH];
    int found = 0;

    printf("Enter the title of the movie: ");
    getchar(); // Yeni satır karakterini temizle
    fgets(searchTitle, sizeof(searchTitle), stdin);
    searchTitle[strcspn(searchTitle, "\n")] = '\0'; // Yeni satır karakterini temizle

    for (int i = 0; i < size; i++)
    {
        if (strcmp(names[i].title, searchTitle) == 0)
        {
            printf("Title: %s\n", names[i].title);
            printf("Rating: %.2f\n", names[i].rating);
            printf("Score: %.2f\n", names[i].score);
            printf("Must See: %s\n", names[i].mustsee);
            printf("URL: %s\n", names[i].url);

            printf("Genres: ");
            if (names[i].genre != NULL)
            {
                for (int j = 0; names[i].genre[j] != NULL; j++)
                {
                    printf("%s", names[i].genre[j]);
                    if (names[i].genre[j + 1] != NULL)
                    {
                        printf(", ");
                    }
                }
            }
            printf("\n");

            printf("Directors: ");
            if (names[i].directors != NULL)
            {
                for (int j = 0; names[i].directors[j] != NULL; j++)
                {
                    printf("%s", names[i].directors[j]);
                    if (names[i].directors[j + 1] != NULL)
                    {
                        printf(", ");
                    }
                }
            }
            printf("\n");

            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Movie not found.\n");
    }
}

void frequencyGenres(struct Name *names, int size)
{

    char uniqueGenres[MAX_GENRES][MAX_GENRE_LENGTH];
    int genreCount = 0;

    for (int i = 0; i < size; i++)
    {
        if (names[i].genre != NULL)
        {
            for (int j = 0; names[i].genre[j] != NULL; j++)
            {
                // Genre'ı zaten eklenip eklenmediğini kontrol ediyoruz
                int found = 0;
                for (int k = 0; k < genreCount; k++)
                {
                    if (strcmp(uniqueGenres[k], names[i].genre[j]) == 0)
                    {
                        found = 1;
                        break;
                    }
                }
                if (!found && genreCount < MAX_GENRES)
                {
                    strncpy(uniqueGenres[genreCount], names[i].genre[j], MAX_GENRE_LENGTH - 1);
                    uniqueGenres[genreCount][MAX_GENRE_LENGTH - 1] = '\0'; // Güvenlik için sonlandırıcı ekle
                    genreCount++;
                }
            }
        }
    }

    int frequency[MAX_GENRES] = {0};

    for (int i = 0; i < size; i++)
    {
        if (names[i].genre != NULL)
        {
            for (int j = 0; names[i].genre[j] != NULL; j++)
            {
                for (int k = 0; k < genreCount; k++)
                {
                    if (strcmp(names[i].genre[j], uniqueGenres[k]) == 0)
                    {
                        frequency[k]++;
                    }
                }
            }
        }
    }

    printf("Genre Frequencies:\n");
    for (int i = 0; i < genreCount; i++)
    {
        printf("%s: %d\n", uniqueGenres[i], frequency[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        perror("Error opening file.");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int num_movies = 0;
    int capacity = INITIAL_CAPACITY;

    // Dinamik bellek ayırma
    struct Budget *budgets = malloc(sizeof(struct Budget) * capacity);
    struct Name *names = malloc(sizeof(struct Name) * capacity);

    if (budgets == NULL || names == NULL)
    {
        perror("Memory allocation error!");
        cleanup(budgets, names, num_movies, file);
        return 1;
    }

    // İlk satırı okuma (başlık satırı)
    if (fgets(line, sizeof(line), file) == NULL)
    {
        perror("Error reading header.");
        cleanup(budgets, names, num_movies, file);
        return 1;
    }

    while (fgets(line, sizeof(line), file))
    {
        // Satırın sonundaki yeni satır karakterini temizlemek için
        line[strcspn(line, "\n")] = '\0';

        char *token;
        token = strtok(line, ";");

        // Eğer dizi dolmuşsa kapasiteyi 2 katına çıkar
        if (num_movies >= capacity)
        {
            capacity *= 2;
            struct Budget *new_budgets = realloc(budgets, sizeof(struct Budget) * capacity);
            struct Name *new_names = realloc(names, sizeof(struct Name) * capacity);
            if (new_budgets == NULL || new_names == NULL)
            {
                perror("Memory reallocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            budgets = new_budgets;
            names = new_names;
        }

        // Budget structını doldurma
        if (token != NULL)
        {
            // Budget (1. sırada)
            budgets[num_movies].budget = atoi(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Title (2. sırada)
            budgets[num_movies].title = strdup(token);
            if (budgets[num_movies].title == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            names[num_movies].title = strdup(token);
            if (names[num_movies].title == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Title Type (3. sırada)
            budgets[num_movies].titletype = strdup(token);
            if (budgets[num_movies].titletype == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Directors (4. sırada)
            token = strtok(NULL, ";");
        }
        /*
         names[num_movies].directors = malloc(sizeof(char*) * MAX_DIRECTORS);
            if (names[num_movies].directors == NULL) {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            char *director_token = strtok(token, ",");
            int director_index = 0;
            while (director_token != NULL && director_index < MAX_DIRECTORS) {
                names[num_movies].directors[director_index] = strdup(director_token);
                if (names[num_movies].directors[director_index] == NULL) {
                    perror("Memory allocation error!");
                    cleanup(budgets, names, num_movies, file);
                    return 1;
                }
                director_token = strtok(NULL, ",");
                director_index++;
            }
            names[num_movies].directors[director_index] = NULL; // Sonlandırıcı NULL
        */
        if (token != NULL)
        {
            // Rating (5. sırada)
            names[num_movies].rating = atof(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Runtime (6. sırada)
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Year (7. sırada)
            budgets[num_movies].year = atoi(token);
            token = strtok(NULL, ";");
        }

        if (token != NULL)
        {
            // Genres (8. sırada)
            token = strtok(NULL, ";");
        }
        /*
            names[num_movies].genre = malloc(sizeof(char*) * MAX_GENRES);
            if (names[num_movies].genre == NULL) {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            char *genre_token = copy_and_tokenize(token);
            int genre_index = 0;
            while (genre_token != NULL && genre_index < MAX_GENRES) {
                names[num_movies].genre[genre_index] = strdup(genre_token);
                if (names[num_movies].genre[genre_index] == NULL) {
                    perror("Memory allocation error!");
                    cleanup(budgets, names, num_movies, file);
                    return 1;
                }
                genre_token = strtok(NULL, ",");
                genre_index++;
            }
            names[num_movies].genre[genre_index] = NULL; // Sonlandırıcı NULL
            free(genre_token);
        */

        if (token != NULL)
        {
            // Votes (9. sırada)
            names[num_movies].score = atof(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Top250 (10. sırada)
            budgets[num_movies].top250 = strdup(token);
            if (budgets[num_movies].top250 == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // Must See (11. sırada)
            names[num_movies].mustsee = strdup(token);
            if (names[num_movies].mustsee == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
            token = strtok(NULL, ";");
        }
        if (token != NULL)
        {
            // URL (12. sırada)
            names[num_movies].url = strdup(token);
            if (names[num_movies].url == NULL)
            {
                perror("Memory allocation error!");
                cleanup(budgets, names, num_movies, file);
                return 1;
            }
        }
        num_movies++;
    }

    fclose(file);

    qsort(budgets, num_movies, sizeof(struct Budget), compareBudget);

    int choice;
    do
    {
        displayMenu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            listBudgetArray(budgets, num_movies);
            break;
        case 2:
            listNameArray(names, num_movies);
            break;
        case 3:
            listGenres(names, num_movies);
            break;
        case 4:
            listMoviesThroughYears(budgets, num_movies);
            break;
        case 5:
            listMoviesByScore(names, num_movies);
            break;
        case 6:
            printMovieInfo(names, num_movies);
            break;
        case 7:
            frequencyGenres(names, num_movies);
            break;
        case 8:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
        }
    } while (choice != 8);

    cleanup(budgets, names, num_movies, NULL);

    return 0;
}
