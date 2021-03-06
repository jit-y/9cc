#include "9cc.h"

bool at_eof() {
    return token->kind == TK_EOF;
}


Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));

    tok->kind = kind;
    tok->str = str;
    cur->next = tok;

    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (!strncmp(p, "==", 2)) {
            cur = new_token(TK_RESERVED, cur, p);
            cur->len = strlen("==");
            p += cur->len;

            continue;
        }

        if (!strncmp(p, "!=", 2)) {
            cur = new_token(TK_RESERVED, cur, p);
            cur->len = strlen("!=");
            p += cur->len;

            continue;
        }

        if (!strncmp(p, "<=", 2)) {
            cur = new_token(TK_RESERVED, cur, p);
            cur->len = strlen("<=");
            p += cur->len;

            continue;
        }

        if (!strncmp(p, ">=", 2)) {
            cur = new_token(TK_RESERVED, cur, p);
            cur->len = strlen(">=");
            p += cur->len;

            continue;
        }

        if (strchr("+-*/()<>;=", *p)) {
            cur = new_token(TK_RESERVED, cur, p++);
            cur->len = 1;

            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            cur = ident(p, cur);
            p = p + cur->len;

            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);

            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p);

    return head.next;
}

Token *ident(char *p, Token *cur) {
    int len = 1;

    while (isalpha(p[len]) || isdigit(p[len]) || p[len] == '_')
        len++;

    char *name = strndup(p, len);
    cur = new_token(TK_IDENT, cur, name);
    cur->str = name;
    cur->len = len;

    return cur;
}
