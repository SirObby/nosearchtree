#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include <stdbool.h>
#include <time.h>

#include "config.h"
#include "algorithm.h"

void bot_failure()
{
    exit(-1);
}

bool poll_event(json_t *event);

struct response_data
{
    char *data;
    size_t size;
};

size_t write_response_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{
    struct response_data *data = (struct response_data *)userdata;
    size_t realsize = size * nmemb;

    data->data = realloc(data->data, data->size + realsize + 1);
    if (data->data == NULL)
    {
        fprintf(stderr, "Error allocating memory for response data\n");
        return 0;
    }

    memcpy(&(data->data[data->size]), ptr, realsize);
    data->size += realsize;
    data->data[data->size] = '\0';

    return realsize;
}

void fetch_bot_profile()
{
    CURL *curl;
    CURLcode res;
    char *url = "https://lichess.org/api/account";
    struct curl_slist *headers = NULL;
    struct response_data data = {.data = NULL, .size = 0};

    curl = curl_easy_init();
    if (!curl)
        bot_failure();
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the write function to store the response data in a buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        bot_failure();
    }

    // Print the response data
    printf("%s\n", data.data);

    json_t *root;
    json_error_t error;

    root = json_loads(data.data, 0, &error);

    if (!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        return;
    }

    json_t *id = json_object_get(root, "id");
    json_t *title = json_object_get(root, "title");
    if (json_is_string(id) && json_is_string(title))
    {
        printf("logged in as: %s %s\n", json_string_value(title), json_string_value(id));
    }
    else
    {
        bot_failure();
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(data.data);
}

json_t *send_get_request(char *url)
{
    json_t *root;
    json_error_t error;

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct response_data data = {.data = NULL, .size = 0};

    curl = curl_easy_init();
    if (!curl)
        bot_failure();
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the write function to store the response data in a buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        bot_failure();
    }

    // Print the response data
    printf("%s\n", data.data);

    root = json_loads(data.data, 0, &error);

    if (!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        exit(1);
        // return 1;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(data.data);

    return root;
}

json_t *send_post_request(char *url, char *post_data)
{
    json_t *root;
    json_error_t error;

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct response_data data = {.data = NULL, .size = 0};

    curl = curl_easy_init();
    if (!curl)
        bot_failure();
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the POST data
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

    // Set the write function to store the response data in a buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        bot_failure();
    }

    // Print the response data
    printf("%s\n", data.data);

    root = json_loads(data.data, 0, &error);

    if (!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        exit(1);
        // return 1;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(data.data);

    return root;
}

json_t *send_custom_post_request(char *url, char *post_data, char *auth_token_two)
{
    json_t *root;
    json_error_t error;

    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;
    struct response_data data = {.data = NULL, .size = 0};

    curl = curl_easy_init();
    if (!curl)
        bot_failure();
    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_token_two);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the POST data
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);

    // Set the write function to store the response data in a buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        bot_failure();
    }

    // Print the response data
    printf("%s\n", data.data);

    root = json_loads(data.data, 0, &error);

    if (!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        exit(1);
        // return 1;
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(data.data);

    return root;
}

/*

    some global variables

*/

char *game_id = "";

size_t stream_response_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{

    // Get the current time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Print the timestamp and the data to the console
    printf("[%02d:%02d:%02d] %.*s", tm->tm_hour, tm->tm_min, tm->tm_sec, size * nmemb, (char *)ptr);

    // Check if the received data is empty
    if (size * nmemb == 0 || ((char *)ptr)[0] == '\n')
    {
        return size * nmemb;
    }

    // Parse the received data as JSON
    json_error_t error;
    json_t *root = json_loadb(ptr, size * nmemb, 0, &error);

    // Check for parsing errors
    if (!root)
    {
        fprintf(stderr, "Failed to parse JSON: %s\n", error.text);
        return 0;
    }
    else
    {
        // Print the parsed data to the console
        printf("%s\n", json_dumps(root, JSON_INDENT(2)));

        // we are not going to stop the stream.
        /*if (*/ poll_event(root); /* == false) return 0;*/ // hacky way to end stream, as a 0 size*nmemb will mean that curl was unable
        // to write the data as such, causing a curl error, ending the stream.

        // Clean up
        json_decref(root);
    }

    // Return the size of the data
    return size * nmemb;
}

void stream_lichess_events()
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }

    // Set the URL
    curl_easy_setopt(curl, CURLOPT_URL, "https://lichess.org/api/stream/event");

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the write function to stream the response data to the console
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_response_data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}

int main(int argc, char **argv)
{
    fetch_bot_profile();

    /*json_t *challenge = send_post_request("https://lichess.org/api/challenge/sir_obsidian", "");

    json_t *challenge_object = json_object_get(challenge, "challenge");

    printf("challenge: https://lichess.org/%s\n", json_string_value(json_object_get(challenge_object, "id")));*/
    // we shouldn't do this.

    while (true)
    {
        // hand over the bot to the events stream.
        // WE SHOULD NEVER CRASH STREAM LICHESS EVENTS LMFAO
        stream_lichess_events();
        // begin game stream.
        // stream_game_events();
    }

    return 0;
}

size_t stream_game_response_data(void *ptr, size_t size, size_t nmemb, void *userdata)
{

    // Get the current time
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    // Print the timestamp and the data to the console
    printf("GAME:[%02d:%02d:%02d] %.*s", tm->tm_hour, tm->tm_min, tm->tm_sec, size * nmemb, (char *)ptr);

    // Check if the received data is empty
    if (size * nmemb == 0 || ((char *)ptr)[0] == '\n')
    {
        return size * nmemb;
    }

    // Parse the received data as JSON
    json_error_t error;
    json_t *root = json_loadb(ptr, size * nmemb, 0, &error);

    // Check for parsing errors
    if (!root)
    {
        fprintf(stderr, "Failed to parse JSON: %s\n", error.text);
        return 0;
    }
    else
    {
        // Print the parsed data to the console
        printf("%s\n", json_dumps(root, JSON_INDENT(2)));

        /*if (*/ poll_event(root); /* == false) return 0; */ // hacky way to end stream, as a 0 size*nmemb will mean that curl was unable
        // to write the data as such, causing a curl error, ending the stream.

        // Clean up
        json_decref(root);
    }

    // Return the size of the data
    return size * nmemb;
}

void stream_game_events()
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "Failed to initialize curl\n");
        return;
    }

    char url[256];
    // Set the URL
    // vsprintf(url, "https://lichess.org/api/bot/game/stream/%s", );

    char send_accept;
    int result = snprintf(url, sizeof(url), "https://lichess.org/api/bot/game/stream/%s", game_id);
    if (result >= sizeof(url))
    {
        fprintf(stderr, "Formatted string truncated\n");
        return;
    }
    printf("%s\n", url);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    // Set the Authorization header
    headers = curl_slist_append(headers, auth_header);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the write function to stream the response data to the console
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, stream_game_response_data);

    // Perform the request
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    // Clean up
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
}

/*

    Calling the algorithm

*/

algorithm a;

bool are_we_ingame = false;

bool poll_event(json_t *event)
{
    // OHHH THIS SHIT IS GOING TO GET FUCKING INTERESTING LMAO
    char *type = json_string_value(json_object_get(event, "type"));

    printf("%s\n", type);

    if (strcmp(type, "challenge") == 0)
    {
        printf("challenge has been received, have to check it\n");

        if (strcmp(json_string_value(json_object_get(json_object_get(event, "challenge"), "speed")), "correspondence") != 0)
        {

            printf("declining challenge %s\n", json_string_value(json_object_get(json_object_get(event, "challenge"), "id")));

            char send_decline[256];
            int result = snprintf(send_decline, sizeof(send_decline), "https://lichess.org/api/challenge/%s/decline", json_string_value(json_object_get(json_object_get(event, "challenge"), "id")));
            if (result >= sizeof(send_decline))
            {
                fprintf(stderr, "Formatted string truncated\n");
                return true;
            }
            printf("%s\n", send_decline);

            send_post_request(send_decline, "reason=timeControl;");
        }
        else
        {

            printf("accepting challenge %s\n", json_string_value(json_object_get(json_object_get(event, "challenge"), "id")));

            char send_accept[256];
            int result = snprintf(send_accept, sizeof(send_accept), "https://lichess.org/api/challenge/%s/accept", json_string_value(json_object_get(json_object_get(event, "challenge"), "id")));
            if (result >= sizeof(send_accept))
            {
                fprintf(stderr, "Formatted string truncated\n");
                return true;
            }
            printf("%s\n", send_accept);

            send_post_request(send_accept, "");
        }
    }

    if (strcmp(type, "gameStart") == 0)
    {
        are_we_ingame = true;
        printf("game has begun %s\n", json_string_value(json_object_get(json_object_get(event, "game"), "gameId")));
        // strcpy(game_id, json_string_value(json_object_get(json_object_get(event, "game"), "gameId")));
        game_id = strdup(json_string_value(json_object_get(json_object_get(event, "game"), "gameId")));
        // replace it with something

        // ""
        char request_url[256];
        int result = snprintf(request_url, sizeof(request_url), "https://lichess.org/api/bot/game/%s/chat", json_string_value(json_object_get(json_object_get(event, "game"), "gameId")));
        if (result >= sizeof(request_url))
        {
            fprintf(stderr, "Formatted string truncated\n");
            return true;
        }

        send_post_request(request_url, "room=player;text=hello world");

        printf("create algorithm\n");
        create_algorithm(&a);

        if (begin_game(&a, json_string_value(json_object_get(json_object_get(event, "game"), "fen")), (strcmp(json_string_value(json_object_get(json_object_get(event, "game"), "color")), "white") == 0) ? WHITE : BLACK) != 0)
        {
            bot_failure();
        };

        printf("starting as: %s", json_string_value(json_object_get(json_object_get(event, "game"), "color")));
        if (strcmp(json_string_value(json_object_get(json_object_get(event, "game"), "color")), "white") == 0)
        {
            printf("we are the white pieces, we move first\n");
            // UCI move that the algorithm has thought.
            char *uci = think(&a);

            char move_url[256];
            int result_move = snprintf(move_url, sizeof(move_url), "https://lichess.org/api/bot/game/%s/move/%s", json_string_value(json_object_get(json_object_get(event, "game"), "gameId")), uci);
            if (result_move >= sizeof(move_url))
            {
                fprintf(stderr, "Formatted string truncated\n");
                return true;
            }

            send_post_request(move_url, "");
        }

        stream_game_events();

        return false;
    }
    if (strcmp(type, "gameFinish") == 0)
    {
        are_we_ingame = false;
        printf("game has ended\n");
        strcpy(game_id, ""); // replace it with nothing
    }
    // A PIECE HAS MOVED, EITHER WE DID IT, OR THEY DID.
    if (strcmp(type, "gameState") == 0)
    {
        // moves: "e2e4"
        game_state(json_string_value(json_object_get(event, "moves")), &a);

        // UCI move that the algorithm has thought.
        char *uci = think(&a);

        char move_url[256];
        int result_move = snprintf(move_url, sizeof(move_url), "https://lichess.org/api/bot/game/%s/move/%s", json_string_value(json_object_get(json_object_get(event, "game"), "gameId")), uci);
        if (result_move >= sizeof(move_url))
        {
            fprintf(stderr, "Formatted string truncated\n");
            return true;
        }

        send_post_request(move_url, "");
    }
    return true;
}