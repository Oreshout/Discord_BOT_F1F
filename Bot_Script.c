         
/**         *****************************************************                                                                                           
 *          *              BOT DISCORD F1F PRONOS               *
 *          *****************************************************
*/

/**
 * @see Discord server : https://discord.gg/6XwJeCraWN
 * Owner : @whisper9741
 * @author : @matt_karting
 * @file Using concord (discord.c) by Cogmasters
 * Link : https://github.com/Cogmasters/concord.git
*/

/**
 * @defgroup standard libraries of C language
 * @see <stdio.h> <string.h> 
 * 
 * @brief use commands
 */
#include <stdio.h>
#include <string.h>

/**
 * @def library of discord.c
 * @see <discord.h> 
 * 
 * @brief download the link above to find it
 */
#include "discord.h"


/**
 * @defgroup limit of actions
 * @see MAX_USERS MAX_NAMES USERNAME
 * 
 * @brief MAX_USERS is to put a limit of action / MAX_NAME is the lenght of the name of the driver /
 * USERNAME is the lenght of the client's name / MAX_PRONOSTIC is the number of pronostic accept by the bot
 */
#define MAX_USERS 5000
#define MAX_NAME 50
#define USERNAME 100
#define MAX_PRONOSTIC 500 

/**
 *          ****************
 *          *  PROTOTYPES  *
 *          ****************
*/

/**
 * @brief Create a pronostic
 * @see ajouter_procedure()
 * 
 * @param username This is the username of the client
 * @param premier This the winner of the race according to the client
 * @param deuxieme This the second of the race according to the client
 * @param troisieme This the third of the race according to the client
 * @param meilleur_tour This is the driver who did the fastest lap in the race according to the client
 * @note the procedure call a file "pronos.txt"
 */
void ajouter_pronostic(const char *username, const char *premier, const char *deuxieme, const char *troisieme, const char *meilleur_tour);


/**
 * @brief Input after the race the winner, second, third and the fastest lap
 * @see commande_admin()
 * 
 * @param premier This the winner of the race 
 * @param deuxieme This the second of the race 
 * @param troisieme This the third of the race 
 * @param meilleur_tour This is the driver who did the fastest lap in the race
 * @param type This is for the type of the step / event of the week-end : Race, Sprint Race, Qualification
 * @note the procedure call a file "resultats.txt"
 */
void commande_admin(const char *premier, const char *deuxieme, const char *troisieme, const char *meilleur_tour, const char *type);


/**
 * @brief To compute the score of the client after every race
 * @see calculer_score()
 * 
 * @note call 2 files "resultats.txt" and "pronos.txt" to compare it and gives points 
 */
void calculer_score();


/**
 * @brief link the code with all the discord librairies
 * @see on_interaction()
 * 
 * @param discord a struct with a pointer *client
 * @param discord_interraction a struct with a pointer *event
 * @note need concord to work
 */
void on_interaction(struct discord *client, const struct discord_interraction *event);

/**
 * @brief this the struct of pronostic call just after the line
 * @see typedef struct{}
 * 
 * @param pronostics[MAX_PRONOSTIC]
 */
pronostic prosnostics[MAX_PRONOSTIC];

/**
 * @brief 
 * 
 * 
 */
struct discord_interaction_response response;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int nombre_pronostic=0;


/**
 * @brief struct of variables 
 * 
 * @see typedef{} struct and pronostic
 */
typedef struct{ 

    char username[USERNAME];
    char premier[MAX_NAME];
    char deuxieme[MAX_NAME];
    char troisieme[MAX_NAME];
    char meilleur_tour[MAX_NAME];
    char type[50];
}pronostic;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void ajouter_pronostic(const char *username, const char *premier, const char *deuxieme, const char *troisieme, const char *meilleur_tour)
{

    if(nombre_pronostic > MAX_USERS)
    {
        printf("Erreur : nombre de pronostiques atteint.");
    }
    else
    {
        strcpy(pronostics[nombre_pronostic].username, username);                //  Copy the string input by the client          
        strcpy(pronostics[nombre_pronostic].premier, premier);                  //  in the file "pronos.txt" it takes 5 variables
        strcpy(pronostics[nombre_pronostic].deuxieme, deuxieme);                //  the username, the winner, the second, the third
        strcpy(pronostics[nombre_pronostic].troiseme, troisieme);               //  and the best lap according to the client
        strcpy(pronostics[nombre_pronostic].meilleur_tour, meilleur_tour);      //
        nombre_pronostic++;

        FILE *fichier = fopen("pronos.txt", "a");                              // Open the file "pronos.txt" and have all the permissions
    
        if(fichier)
        {
            fprintf(fichier, "%s %s %s %s %s\n", username, premier, deuxieme, troisieme, meilleur_tour); // Print all the informations of the command in the file
            fclose(fichier);                                                                             // and close the file at the end
        }
        else
        {
            printf("ERROR : Je ne peux pas ouvrir le fichier.\n");
        }
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void commande_admin(const char *premier, const char *deuxieme, const char *troisieme, const char *meilleur_tour, const char *type)
{
    strcpy(pronostic[nombre_pronostic].premier, premier);                     // Copy the string input by the administrator
    strcpy(pronostic[nombre_pronostic].deuxieme, deuxieme);                   // in the file "resultats.txt" it takes 5 variables
    strcpy(pronostic[nombre_pronostic].troiseme, troisieme);                  // the winner, the second, the third, the best lap and 
    strcpy(pronostic[nombre_pronostic].meilleur_tour, meilleur_tour);         // the type of the race. Now this the true result at the end of
    strcpy(pronostic[nombre_pronostic].type, type);                           // the race

    FILE *fichier = fopen("resultats.txt", "w");                              // Open the file "resultats.txt" and have the permission of write in it
    if(fichier)
    {
        fprintf(fichier, "%s %s %s %s %s\n", premier, deuxieme, troisieme, meilleur_tour, type);  // print the answers in the file
        fclose(fichier);                                                                          // and close it
    }
    else
    {
        printf("ERROR : Je ne peux pas écrire dans le fichier\n");
    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void calculer_score()
{
    FILE *fichier_pronos = fopen("pronos.txt", "r");          // Open the file "pronos.txt" with the permission to read it
    FILE *fichier_resultats = fopen("resultats.txt", "r");    // Open the file "resultats.txt" with the permission to read it

    if(!fichier_pronos || !fichier_resultats)
    {
        printf("ERROR : Impossible d'ouvrir les fichiers.\n");
    } 
    else if(fichier_pronos == NULL || fichier_resultats == NULL)
    {
        printf("ERROR : Impossible d'ouvrir le fichier.\n");
    }

    char correct_premier[MAX_NAME];
    char correct_deuxieme[MAX_NAME];
    char correct_troisieme[MAX_NAME];
    char correct_meilleur_tour[MAX_NAME];
    char type[50];
    // Take the correct result of the race
    fscanf(fichier_resultats, "%s %s %s %s %s", correct_premier, correct_deuxieme, correct_troisieme, correct_meilleur_tour, type);

    char username[USERNAME];
    char premier[MAX_NAME];
    char deuxieme[MAX_NAME];
    char troisieme[MAX_NAME];
    char meilleur_tour[MAX_NAME];
    float score_course = 0;                                                         // Counter the score of the client

    /**
     * @brief compare the correct result input by the admin with the pronostic input by the client
     * 
     * @param fichier_pronos
     * @note For the moment there is only 2 way to write Course (Race) and Sprint Course (Sprint Race) and 4 way to write Qualification 
     */
    while(fscanf(fichier_pronos, "%s %s %s %s %s", username, premier, deuxieme, troisieme, meilleur_tour) == 5)
    {
        float correct_result = 0;                                                   // Counter for the number of good result
        float score_noob = 0;                                                       // Counter for the number of good answer but not at the good position
        if(strcmp(type, "course") == 0 || strcmp(type, "Course") == 0)
        {
            if(strcmp(premier, correct_premier) == 0)
                correct_result++;
            if(strcmp(deuxieme, correct_deuxieme) == 0)
                correct_result++;
            if(strcmp(troisieme, correct_troisieme) == 0) 
                correct_result++;
            if(strcmp(meilleur_tour, correct_meilleur_tour) == 0)
                correct_result++;

            if(correct_result == 1)
                score_course += 2;
            else if(correct_result == 2)
                score_course += 4;
            else if(correct_result == 3)
                score_course += 6;
            else if(correct_result == 4)
                score_course += 15;

            if(strcmp(premier, correct_deuxieme) == 0 || strcmp(premier, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(deuxieme, correct_premier) == 0 || strcmp(deuxieme, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(troisieme, correct_premier) == 0 || strcmp(troisieme, correct_deuxieme) == 0)
                score_noob++;

            if(score_noob == 1)
                score_course += 1;
            else if(score_noob == 2)
                score_course += 2;
            else if(score_noob == 3)
                score_course += 3;  
        }

        if(strcmp(type, "course sprint") == 0 || strcmp(type, "Course Sprint") == 0)
        {
            float score_result;
            float score_noob;

            if(strcmp(premier, correct_premier) == 0)
                correct_result++;
            if(strcmp(deuxieme, correct_deuxieme) == 0)
                correct_result++;
            if(strcmp(troisieme, correct_troisieme) == 0) 
                correct_result++;

            if(correct_result == 1)
                score_course += 1;
            else if(correct_result == 2)
                score_course += 2;
            else if(correct_result == 3)
                score_course += 5;

            if(strcmp(premier, correct_deuxieme) == 0 || strcmp(premier, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(deuxieme, correct_premier) == 0 || strcmp(deuxieme, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(troisieme, correct_premier) == 0 || strcmp(troisieme, correct_deuxieme) == 0)
                score_noob++;

            if(score_noob == 1)
                score_course += 0.5;
            else if(score_noob == 2)
                score_course += 3;
            else if(score_noob == 3)
                score_course += 4.5;  
        }

        if(strcmp(type, "qualif") == 0 || strcmp(type, "Qualif") == 0 || strcmp(type,"qualification") == 0 || strcmp(type, "Qualification") == 0)
        {
            float score_result;
            float score_noob;

            if(strcmp(premier, correct_premier) == 0)
                correct_result++;
            if(strcmp(deuxieme, correct_deuxieme) == 0)
                correct_result++;
            if(strcmp(troisieme, correct_troisieme) == 0) 
                correct_result++;

            if(correct_result == 1)
                score_course += 1;
            else if(correct_result == 2)
                score_course += 2;
            else if(correct_result == 3)
                score_course += 5;

            if(strcmp(premier, correct_deuxieme) == 0 || strcmp(premier, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(deuxieme, correct_premier) == 0 || strcmp(deuxieme, correct_troisieme) == 0)
                score_noob++;
            if(strcmp(troisieme, correct_premier) == 0 || strcmp(troisieme, correct_deuxieme) == 0)
                score_noob++;

            if(score_noob == 1)
                score_course += 0.5;
            else if(score_noob == 2)
                score_course += 3;
            else if(score_noob == 3)
                score_course += 4.5;  
        }

        char msg_bot[256];
        snprintf(msg_bot, sizeof(msg_bot), "**%s** : %.2f points pour la %s\n", username, score_course, type);

        response.data.content = msg_bot;
    }

    fclose(fichier_pronos);
    fclose(fichier_resultats);
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void on_interaction(struct discord *client, const struct discord_interraction *event)
{
    if(event->type == DISCORD_INTERACTION_APPLICATION_COMMAND)
    {
        const char *command_name = event->data->name;
        const struct discord_application_command_interaction_data_option *command_options = event->data->options;
        int *nombre_elements = event->data->options->size;

        struct discord_interaction_response response;

        /**
         * @brief command discord used by the client to put their pronostic
         * 
         * @param command_name 
         * @param command_options a struct to put differrents options in the command
         * @note call de procedure ajouter_pronostic()
         */
        if(strcmp(command_name, "pronos") == 0)
        {
            const char *username = event->member->user->username;
            const char *premier = command_options[0].value;
            const char *deuxieme = command_options[1].value;
            const char *troisieme = command_options[2].value;
            const char *meilleur_tour = command_options[3].value;

            ajouter_pronostic(username, premier, deuxieme, troisieme, meilleur_tour);

            char msg_bot[256];
            snprintf(msg_bot, sizeof(msg_bot), "**%s** a enregistré son pronostic :\n %s :first_place:\n %s :second_place:\n %s :third_place:\n %s meilleur tour",
            username, premier, deuxieme, troisieme, meilleur_tour);

            response.type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE;
            response.data.content = msg_bot;

        }

        /**
         * @brief command discord used by the admin to put the correct result of the race
         * 
         * @param command_name 
         * @param command_options a struct to put differents options in the command
         * @note call the procedure commande_admin()
         */
        else if (strcmp(command_name, "admin_result") == 0)
        {
            const char *premier = command_options[0].value;
            const char *deuxieme = command_options[1].value;
            const char *troisieme = command_options[2].value;
            const char *meilleur_tour = command_options[3].value;
            const char *type = command_options[4].value;

            commande_admin(premier, deuxieme, troisieme, meilleur_tour, type);

            char msg_bot[256];
            snprintf(msg_bot, sizeof(msg_bot), ":white_check_mark: **Résultat Enregistré**.\n");

            response.data.content = msg_bot;
        }

        /**
         * @brief command to show all the commands of the bot
         * 
         * @note work alone
         */
        else if(strcmp(command_name, "help") == 0)
        {
            char msg_bot[256];
            snprintf(msg_bot, sizeof(msg_bot)"**Liste des commandes :**\n"
                     "`/pronos pilote1 pilote2 pilote3 meilleur_tour` - Enregistrer un pronostic.\n"
                     "`/admin_result pilote1 pilote2 pilote3 meilleur_tour type` - Ajouter les résultats.\n"
                     "`/scores` - Voir le classement.\n");

            response.type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE;
            response.data.content = msg_bot;
        }
        else
        {
            response.type = DISCORD_INTERACTION_CALLBACK_CHANNEL_MESSAGE_WITH_SOURCE;
            response.data.content = ":x: Commande inconnue.\n **/help** pour voir toute les commandes.\n";
        }
        
        discord_create_interaction_response(client, event->id, event->token, &response, NULL);

    }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main()
{
    struct discord *client = discord_init("config.json");
    
    if(!client)
    {
        printf("ERROR : Impossible d'initialiser le bot discord !\n");
    }

    discord_set_on_integration_create(client, on_interaction);

    discord_run(client);

    discord_cleanup(client);

    return 0;
}
