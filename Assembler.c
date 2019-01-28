#include < stdio.h > #include < string.h > #include < stdlib.h >

  struct mnemonic_opcode_table {
    char ins[20];
    int opcode, len, link, type;
  }
mot[26];

struct symbol_table {
  char sym[20];
  int address;
}
symt[20];

struct tc {
  char lc[5], f1[5], f2[5], f3[5];
}
target[20];

struct register_table {
  char reg[10];
  int reg_code;
}
rt[4];

struct intermediate_code {
  int lc, opcode, reg_code, sym_index;
  char type[4], slc;
}
ic[30];

struct relational_oprator_table {
  char operator[4];
}
rot[7];

struct literal_tabel {
  int address;
  char literal[4];
}
lt[10];

struct pool_tabel {
  int index;
}
pt[10];

int literal_search(char str[4]);
void rot_initialize();
void literalt_initialize();
void create_mot();
int hash_search(int x, char str[50]);
void print_mot();
void print_symbol_table();
int sym_search(char str[20]);
void regt_initialize();
void symt_initialize();
void print_ic();
int reg_search(char str[10]);
int check_type(char str[10]);
void target_code();
void print_target_code();
void print_literal_tabel();
void print_pool_tabel();

int main() {
  int lc, diff, result, sym_index = 0, literal_index = 0, i, j, pool_index = 0;
  int cnt;
  char str[50];
  char line[128]; 
  char str1[50], str2[50], str3[50], str4[50];
  static
  const char filename[] = "alp1.txt";
  FILE * file = fopen(filename, "r");
  create_mot();
  print_mot();
  symt_initialize();
  regt_initialize();
  rot_initialize();
  literalt_initialize();
  int ic_index = 0;
  if (file != NULL) {
    while (fgets(line, sizeof line, file) != NULL) /* read a line */ {
      cnt = sscanf(line, "%s %s %s %s", str1, str2, str3, str4);
      result = hash((str1[0] - 65), str1);
      if (result == -1) // first word is not an instruction
      {
        if (sym_search(str1) == -1) // checks if label is already entered
          strcpy(symt[sym_index++].sym, str1); // if not found in symbol table then  enter label in symbol table
        result = hash(str2[0] - 65, str2);
        if (result == -1) // invalid line of code, write an error over here*
          break;
      }

      if (mot[result].type == 1) // Assembler Directive
      {
        if (mot[result].opcode == 5 || mot[result].opcode == 4) // START and ORGIN
        {
          lc = atoi(str2);
        } else if (mot[result].opcode == 1) // EQU
        {
          symt[sym_search(str1)].address = symt[sym_search(str3)].address;
        } else if (mot[result].opcode == 2 || mot[result].opcode == 3) // END & LTORG
        {
          for (literal_index = 0; lt[literal_index].address != -1; literal_index++); // literal tabel and pool tabel processing
          if (lt[literal_index].literal[0] != '\0')
            pt[pool_index].index = literal_index; // enter in pool tabel
          for (i = 0; lt[literal_index].literal[0] != '\0'; i++) {
            ic[ic_index].lc = lc;
            ic[ic_index].opcode = 2;
            ic[ic_index].sym_index = atoi(lt[literal_index].literal[2]);
            ic[ic_index].slc = 'l';
            strcpy(ic[ic_index++].type, "DL");
            lt[literal_index].address = lc++;
            literal_index++;
          }

        }
      } else if (mot[result].type == 2) // imperative
      {
        ic[ic_index].lc = lc;
        if (mot[result].opcode == 7 || mot[result].opcode == 9) // PRINT and READ
        {
          if (sym_search(str1) != -1) // label is found
          {
            if (sym_search(str3) == -1) // checks if third word is already entered in symbol table
              strcpy(symt[sym_index++].sym, str3); // enters str3 in symbol table
            if (check_type(str3) == 1) // constant
            {
              ic[ic_index].slc = 'c';
              ic[ic_index].sym_index = lc;
            } else if (check_type(str3) == 0) // symbol
            {
              ic[ic_index].slc = 's';
              ic[ic_index].sym_index = sym_search(str4);
            }
          } else // str1 is not a label
          {
            if (sym_search(str2) == -1) // checks if 2nd word is already entered in symbol table
              strcpy(symt[sym_index++].sym, str2); // enters str2 in symbol table
            if (check_type(str2) == 1) // constant
            {
              ic[ic_index].slc = 'c';
              ic[ic_index].sym_index = lc;
            } else if (check_type(str2) == 0) // symbol
            {
              ic[ic_index].slc = 's';
              ic[ic_index].sym_index = sym_search(str2);
            }
          }
        } else if (mot[result].opcode == 2) // bc
        {
          ic[ic_index].reg_code = rot_search(str2);
          ic[ic_index].slc = 's';
          ic[ic_index].sym_index = sym_search(str3);

          // relational operator table
        } else // other register instructions
        {
          if (sym_search(str1) != -1) // label is found
          {
            symt[sym_search(str1)].address = lc;
            ic[ic_index].reg_code = reg_search(str3); // register code in ic
            if (str4[0] == '=') {
              if (literal_search(str4) == -1)
                strcpy(lt[literal_index++].literal, str4);
            } // entering in literal tabel
            else if (sym_search(str4) == -1)
              strcpy(symt[sym_index++].sym, str4); // enter the sym in sym table
            if (check_type(str4) == 1) // constant
            {
              ic[ic_index].slc = 'c';
              ic[ic_index].sym_index = lc;
            } else if (check_type(str4) == 0) // symbol 
            {
              ic[ic_index].slc = 's';
              ic[ic_index].sym_index = sym_search(str4);
            } else if (check_type(str4) == 2) //literal
            {
              ic[ic_index].slc = 'l';
              ic[ic_index].sym_index = literal_search(str4);
            }

          } else // label is not found
          {
            ic[ic_index].reg_code = reg_search(str2); // register code in ic
            if (str3[0] == '=') {
              if (literal_search(str3) == -1)
                strcpy(lt[literal_index++].literal, str3);
            } else if (sym_search(str3) == -1)
              strcpy(symt[sym_index++].sym, str3); // enter the sym in sym table
            if (check_type(str3) == 1) // constant
            {
              ic[ic_index].slc = 'c';
              ic[ic_index].sym_index = lc;
            } else if (check_type(str3) == 0) // symbol
            {
              ic[ic_index].slc = 's';
              ic[ic_index].sym_index = sym_search(str3);
            } else if (check_type(str3) == 2) //literal
            {
              ic[ic_index].slc = 'l';
              ic[ic_index].sym_index = literal_search(str3);
            }
          }
        }

        strcpy(ic[ic_index].type, "IS");
        ic[ic_index].opcode = mot[result].opcode;
        lc++;
        ic_index++;
      } else if (mot[result].type == 3) // Declarative Statement
      {
        ic[ic_index].lc = lc;
        strcpy(ic[ic_index].type, "DL");
        ic[ic_index].opcode = mot[result].opcode;
        ic[ic_index].slc = 'c';
        ic[ic_index].sym_index = atoi(str3);
        if (mot[result].opcode == 1) //dc
        {
          symt[sym_search(str1)].address = lc;
          lc++;
        } else if (mot[result].opcode == 2) // ds
        {
          symt[sym_search(str1)].address = lc;
          lc = lc + atoi(str3) + 1;
        }
        ic_index++;
      }

    }
    fclose(file);
  } else {
    perror(filename); 
  }
  print_ic();
  print_symbol_table();
  target_code();
  print_target_code();
  print_literal_tabel();
  print_pool_tabel();
  return 0;
}

void print_mot() {
  int x;
  printf("\nMnemonic\tType\tOpcode\tlength");
  for (x = 0; x < 26; x++) {
    if (mot[x].len != 0) {

      printf("\n%s\t\t", mot[x].ins);
      printf("%d\t", mot[x].type);
      printf("%d\t", mot[x].opcode);
      printf("%d\t", mot[x].len);
      printf("x=%d", x);
    }
  }
}

void print_symbol_table() {
  int x;
  printf("\nSYMBOL TABLE");
  printf("\nSymbol \t Address");
  for (x = 0; strlen(symt[x].sym) != 0; x++) {

    printf("\n%s", symt[x].sym);
    printf("\t%d", symt[x].address);

  }
  printf("\n");
}

void create_mot() {
  // imperative 2
  //ad 1
  // ds 3
  int x;
  for (x = 0; x < 26; x++) {
    mot[x].len = 1;
    mot[x].link = -1;
  }

  strcpy(mot[0].ins, "ADD");
  mot[0].type = 2;
  mot[0].opcode = 1;

  strcpy(mot[1].ins, "BC");
  mot[1].type = 2;
  mot[1].opcode = 2;

  strcpy(mot[2].ins, "COMP");
  mot[2].type = 2;
  mot[2].opcode = 3;

  strcpy(mot[3].ins, "DC");
  mot[3].type = 3;
  mot[3].opcode = 1;
  mot[3].link = 6;

  strcpy(mot[4].ins, "EQU");
  mot[4].type = 1;
  mot[4].opcode = 1;
  mot[4].link = 5;

  strcpy(mot[5].ins, "END");
  mot[5].type = 1;
  mot[4].opcode = 2;

  strcpy(mot[6].ins, "DS");
  mot[6].type = 3;
  mot[6].opcode = 2;
  mot[6].link = 7;

  strcpy(mot[7].ins, "DIV");
  mot[7].type = 2;
  mot[7].opcode = 4;

  strcpy(mot[11].ins, "LTORG");
  mot[11].type = 1;
  mot[11].opcode = 3;

  strcpy(mot[12].ins, "MULT");
  mot[12].type = 2;
  mot[12].opcode = 5;
  mot[12].link = 13;

  strcpy(mot[13].ins, "MOVER");
  mot[13].type = 2;
  mot[13].opcode = 6;
  mot[13].link = 16;

  strcpy(mot[14].ins, "ORIGIN");
  mot[14].type = 1;
  mot[14].opcode = 4;

  strcpy(mot[15].ins, "PRINT");
  mot[15].type = 2;
  mot[15].opcode = 7;

  strcpy(mot[16].ins, "MOVEM");
  mot[16].type = 2;
  mot[16].opcode = 8;

  strcpy(mot[17].ins, "READ");
  mot[17].type = 2;
  mot[17].opcode = 9;

  strcpy(mot[18].ins, "START");
  mot[18].type = 1;
  mot[18].opcode = 5;
  mot[18].link = 19;

  strcpy(mot[19].ins, "STOP");
  mot[19].type = 2;
  mot[19].opcode = 10;
  mot[19].link = 20;

  strcpy(mot[20].ins, "SUB");
  mot[20].type = 2;
  mot[20].opcode = 11;
}

int hash(int x, char str[50]) {
  int ans;
  ans = strcmp(str, mot[x].ins);

  if (ans == 0) {
    return x;
  } else {
    if (mot[x].link == -1) {
      return -1;
    } else if (mot[x].link != -1) {
      hash(mot[x].link, str);
    }
  }
}

int sym_search(char str[20]) {

  int i;
  for (i = 0; i < 5; i++) {
    if (strcmp(symt[i].sym, str) == 0) {
      return i;
    }
  }
  return -1;
}

void symt_initialize() {
  int i;
  for (i = 0; i < 21; i++) {
    symt[i].address = -1;
  }
}
void regt_initialize() {
  strcpy(rt[0].reg, "AREG");
  rt[0].reg_code = 01;
  strcpy(rt[1].reg, "BREG");
  rt[1].reg_code = 02;
  strcpy(rt[2].reg, "CREG");
  rt[2].reg_code = 03;
  strcpy(rt[3].reg, "DREG");
  rt[3].reg_code = 04;
}

void rot_initialize() {
  strcpy(rot[0].operator, "LE");
  strcpy(rot[1].operator, "GE");
  strcpy(rot[2].operator, "NE");
  strcpy(rot[3].operator, "LT");
  strcpy(rot[4].operator, "GT");
  strcpy(rot[5].operator, "EQ");
  strcpy(rot[6].operator, "ANY");
}

void print_ic() {
  int i;
  for (i = 0; ic[i].type[0] != '\0'; i++) {
    printf("\n%d", ic[i].lc);
    printf("\t(%s,%d)\t", ic[i].type, ic[i].opcode);
    if (ic[i].reg_code != 0)
      printf("%d", ic[i].reg_code);
    printf("\t");
    if (ic[i].slc != '\0')
      printf("(%c,%d)", ic[i].slc, ic[i].sym_index);
  }
}

int reg_search(char str[10]) {

  int i;

  for (i = 0; i < 4; i++) {
    if (strcmp(rt[i].reg, str) == 0) {

      return rt[i].reg_code;
    }
  }
  return 0;
}

int rot_search(char str[4]) {
  int i;
  for (i = 0; i < 7; i++) {
    if (strcmp(rot[i].operator, str) == 0) {
      return i + 1;
    }
  }
  return 0;
}

int check_type(char str[10]) {
  int i;
  if (str[0] == '=')
    return 2;

  for (i = 0; i <= strlen(str) - 1; i++) {
    if ((int) str[i] < 47 || (int) str[i] > 57)
      return 0;
  }
  return 1;
}

int literal_search(char str[4]) {
  int i;
  for (i = 0; i < 11; i++) {
    if (strcmp(lt[i].literal, str) == 0) {
      return i;
    }
  }
  return -1;
}

void print_literal_tabel() {
  int i = 0;
  printf("\nLITERAL TABEL\n");
  for (i = 0; lt[i].literal[0] != '\0'; i++)
    printf("\n%s\t%d", lt[i].literal, lt[i].address);
}

void print_pool_tabel() {
  int i = 0;
  printf("\nPOOL TABEL\n");
  for (i = 0; pt[i].index != -1; i++)
    printf("\n%d", pt[i].index);
}

void literalt_initialize() {
  int i;
  for (i = 0; i < 11; i++) {
    lt[i].address = -1;
    pt[i].index = -1;
  }
}

void target_code() {
  int i;
  for (i = 0; ic[i].type[0] != '\0'; i++) {

    snprintf(target[i].lc, sizeof(target[i].lc), "%d", ic[i].lc);

    if (strcmp(ic[i].type, "IS") == 0) // imperative statement
    {
      snprintf(target[i].f1, sizeof(target[i].f1), "%d", ic[i].opcode);
      if (ic[i].reg_code != 0)
        snprintf(target[i].f2, sizeof(target[i].f2), "%d", ic[i].reg_code);
      if (ic[i].slc == 's') {
        snprintf(target[i].f3, sizeof(target[i].f3), "%d", symt[ic[i].sym_index].address);
      } else if (ic[i].slc == 'c') {
        snprintf(target[i].f3, sizeof(target[i].f3), "%d", ic[i].sym_index);
      }
    } else if (strcmp(ic[i].type, "DL") == 0) // decleative statement
    {
      if (ic[i].opcode == 1) {
        snprintf(target[i].f3, sizeof(target[i].f3), "%d", ic[i].sym_index);
        strcpy(target[i].f1, "00");
        strcpy(target[i].f2, "00");
      }
    }
  }
}

void print_target_code() {
  int i;
  for (i = 0; target[i].lc[0] != '\0'; i++) {
    printf("\n%s\t%s\t%s\t%s", target[i].lc, target[i].f1, target[i].f2, target[i].f3);

  }
}
