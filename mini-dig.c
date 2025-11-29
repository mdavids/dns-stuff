
// mini-dig.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <resolv.h>
#include <arpa/nameser.h>
#include <netinet/in.h>
#include <netdb.h>

#ifndef C_IN
#define C_IN 1
#endif

#ifndef T_TXT
#define T_TXT 16
#endif

// Laat deze helper staan voor de veilige modus
#ifndef USE_SIMPLE_TXT
static void print_quoted_chunk(const unsigned char *s, int n) {
    putchar('"');
    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        if (c == '"' || c == '\\') {
            putchar('\\');
            putchar(c);
        } else if (c < 32 || c >= 127) {
            printf("\\%03o", c);
        } else {
            putchar(c);
        }
    }
    putchar('"');
}
#endif

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <domain_name>\n", argv[0]);
        return 1;
    }

    const char *domain = argv[1];

    printf("; <<>> C TXT Lookup Tool - dig-like utility <<>>\n");
    printf("; Searching for TXT records for: %s\n", domain);
    printf(";; QUESTION SECTION:\n");
    printf(";%s.\t\tIN\tTXT\n\n", domain);
    printf(";; ANSWER SECTION:\n");

    res_state state = malloc(sizeof(*state));
    if (!state) {
        fprintf(stderr, "Allocation failed\n");
        return 1;
    }
    if (res_ninit(state) != 0) {
        fprintf(stderr, "Resolver init failed\n");
        free(state);
        return 1;
    }
#ifdef RES_USE_EDNS0
    state->options |= RES_USE_EDNS0;
#endif

    size_t bufsize = 65536;
    unsigned char *response = (unsigned char *)malloc(bufsize);
    if (!response) {
        fprintf(stderr, "Allocation failed\n");
        res_nclose(state);
        free(state);
        return 1;
    }

    int response_len = res_nquery(state, domain, C_IN, T_TXT, response, (int)bufsize);
    if (response_len < 0) {
        fprintf(stderr, ";; ERROR: Could not retrieve TXT records for %s (h_errno=%d)\n", domain, h_errno);
        free(response);
        res_nclose(state);
        free(state);
        return 1;
    }

    ns_msg handle;
    if (ns_initparse(response, response_len, &handle) < 0) {
        perror("ns_initparse");
        free(response);
        res_nclose(state);
        free(state);
        return 1;
    }

    int ancount = ns_msg_count(handle, ns_s_an);
    if (ancount == 0) {
        printf(";; No TXT records found for %s\n", domain);
    } else {
        for (int i = 0; i < ancount; i++) {
            ns_rr rr;
            if (ns_parserr(&handle, ns_s_an, i, &rr) == 0) {
                const unsigned char *rdata = ns_rr_rdata(rr);
                int rdlen = ns_rr_rdlen(rr);

                int j = 0;
                printf("%s.\t\tIN\tTXT\t", domain);
                int first = 1;
                while (j < rdlen) {
                    int chunk_len = rdata[j];
                    j++;
                    if (chunk_len < 0 || j + chunk_len > rdlen) {
                        fprintf(stderr, "\n;; WARN: malformed TXT RDATA (len=%d, rdlen=%d)\n", chunk_len, rdlen);
                        break;
                    }
                    if (!first) putchar(' ');
#ifdef USE_SIMPLE_TXT
                    // Eenvoudig, zoals eerder (geen extra escaping)
                    printf("\"%.*s\"", chunk_len, (const char *)(rdata + j));
#else
                    // Veilige gequote variant
                    print_quoted_chunk(rdata + j, chunk_len);
#endif
                    first = 0;
                    j += chunk_len;
                }
                putchar('\n');
            }
        }
    }

    printf("\n;; Query completed.\n");

    free(response);
    res_nclose(state);
    free(state);
    return 0;
}
