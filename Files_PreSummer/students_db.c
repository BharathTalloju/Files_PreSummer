#include <stdlib.h>
#include <stdio.h>

//Data structures
typedef struct subject_name{
    char name[20];
}subject_name;

struct student_node{
    char name[40];
    int *marks;
    struct student_node *next;
};

struct students_head{
    struct student_node *first;
    struct student_node *last;
};

struct context{
    char file_name[50];
    struct students_head *students_list;
    subject_name *subjects;
    int N;
    struct student_node *append_list;
    int flag_append_list_present;
};


//Function declarations
int is_digit(char c);
void strcpy(char *source, char *dest);
int str_equal(char *s1, char *s2);
void get_all_marks_student(char *s_name, struct students_head *students_list,subject_name *subjects, int N);
void get_max_marks_student(char *s_name, struct students_head *students_list, subject_name *subjects, int N);
void get_min_marks_student(char *s_name, struct students_head *students_list, subject_name *subjects, int N);
void get_marks_subject(struct students_head *students_list, subject_name *subjects, int N);
void get_max_marks_subject_student(struct students_head *students_list, subject_name *subjects, int N);
void get_marks_student_subject(char *s_name, struct students_head *students_list, subject_name *subjects, int N);
void insert(struct students_head *students_list, char *name, int *marks, int N);
void main_menu();
int read_file(char *file_name);
void display_list(struct students_head *students_list,subject_name *subjects, int N);
void get_marks_student_all_subjects(struct students_head *students_list, subject_name *subjects, int N);
void get_average_marks_student(char *s_name, struct students_head *students_list, struct subject_name *subjects, int N);
void add_student();
int name_already_exists(char *s_name);
void save_and_exit();

//Global context
struct context Context = {"\0", NULL, NULL, 0, NULL, 0};


int main(){
    char file_name[50];
    
    printf("Enter file name: \n");
    scanf("%s", file_name);
    strcpy(file_name, Context.file_name);
    
    if(!read_file(file_name)){
        printf("File %s not present\n", file_name);
    }
    else{
        main_menu();
    }
    return 0;
}



void display_list(struct students_head *students_list,subject_name *subjects, int N){
    struct student_node *walker = students_list-> first;
    int i ;
    
    printf("List:-\n");
    printf("                    ");
    for(i = 0 ; i < N ; i++){
        printf("%20s", subjects[i].name);
    }
    printf("\n");
    while(walker){
        printf("%20s", walker-> name);
        for(i = 0 ; i < N; i++){
            printf("%20d ", walker-> marks[i]);
        }    
        printf("\n");
        walker = walker-> next;
    }
    printf("\n\n");
}

void insert(struct students_head *students_list, char *name, int *marks, int N){
    int i;
    struct student_node *new = NULL;
    
    //printf("\nin insert\n");
    new = (struct student_node *)malloc(sizeof(struct student_node));
    strcpy(name, new-> name);
    new-> marks = (int*)calloc(N, sizeof(int));
    new-> next = NULL;
    for(i = 0; i<N; i++){
        new-> marks[i] = marks[i];
    }
    
    
    if(students_list-> first == NULL){
        students_list-> first = new;
    }
    else{
        students_list-> last-> next = new;
    }
    
    
    students_list-> last = new;
}

void main_menu(){
    int opt = 0;
    
    while(opt != 8){
        printf("\t\t\t\tMenu\n");
        printf("\n1. Display List");
        printf("\n2. Get marks of a student");
        printf("\n3.Get max marks of a student");
        printf("\n4. Get marks obtained by a student in a subject");
        printf("\n5. Get average marks of a student");
        printf("\n6. Get marks obtained by all students in a subject");
        printf("\n7. Add student and details");
        printf("\n8. Save and exit");
        printf("\nYour option: ");
        scanf("%d", &opt);
        
        system("clear");
        switch(opt){
            case 1:
                display_list(Context.students_list, Context.subjects, Context.N);
                break;
                
            case 2:
                {
                    char s_name[40];
                    
                    printf("\nEnter the student name: ");
                    scanf("%s", s_name);
                    get_all_marks_student(s_name, Context.students_list, Context.subjects, Context.N);
                }
                break;
                
            case 3:
                {
                    char s_name[40];
                    
                    printf("\nEnter the student name: ");
                    scanf("%s", s_name);
                    get_max_marks_student(s_name, Context.students_list, Context.subjects, Context.N);
                }
                break;
                
            case 4:
                {
                    char s_name[40];
                    
                    printf("\nEnter the student name: ");
                    scanf("%s", s_name);
                    get_marks_student_subject(s_name, Context.students_list, Context.subjects, Context.N);
                }
                break;
                
            case 5:
                {
                    char s_name[40];
                    
                    printf("\nEnter the student name: ");
                    scanf("%s", s_name);
                    get_average_marks_student(s_name, Context.students_list, Context.subjects, Context.N);
                }
                break;
                
            case 6:
                get_marks_subject(Context.students_list, Context.subjects, Context.N);
                break;
                
            case 7:
                add_student();
                break;
            case 8:
                save_and_exit();
                break;
            default:
                printf("1-8 u SOB");
                break;
        }
    }
}

int read_file(char *file_name){
    FILE *fp = NULL;
    int N = 0;
    int i1 = 0;
    int i2 = 0;
    subject_name *subjects = NULL;
    char c;
    struct students_head *students_list = NULL;
    
    students_list = (struct students_head *)malloc(sizeof(struct students_head));
    students_list-> first = NULL;
    students_list-> last = NULL;
    
    if(!(fp = fopen(file_name, "r"))){
        return 0;
    }
    
    //Reading subject names
    fscanf(fp, "%d\n", &N);
    subjects = (subject_name*)calloc(N, sizeof(subject_name));
    printf("no of subjects read : %d\n", N);
    for(i1 = 0 ; i1 < N ; i1++){
        fscanf(fp, "%c", &c);
        i2 = 0;
        while((c!= ',') & (c!= '\r') & (c!= '\n')){
            subjects[i1].name[i2++] = c;
            fscanf(fp, "%c", &c);
        }
        subjects[i1].name[i2] = '\0';
    }
    printf("\nSubjects: ");
    for(i1 = 0 ; i1 < N ; i1++){
        printf("%s, ", subjects[i1].name);
    }
    
    while(!feof(fp)){
        char s_name[40];
        int marks[N];
        int x;
        int error_flag = 0;
        
        fscanf(fp, "%s", &s_name);
        for(x = 0 ; s_name[x] != '\0' ; x++){
            if(is_digit(s_name[x])){
            printf("\nError in format of file");
            error_flag = 1;
            break;
        }
        }
        if(error_flag){
            break;
        }
        printf("\nRead name: %s", s_name);
        printf("\nMarks: ");
        for(i1 = 0 ; i1 < N ; i1++){
            fscanf(fp, "%d,", &marks[i1]);
            printf("%d, ", marks[i1]);
        }
        //getchar();
        printf("\n");
        
        fscanf(fp, "\n");
        insert(students_list, s_name, marks, N);
    }
    Context.students_list = students_list;
    Context.subjects = subjects;
    Context.N = N;
    //printf("\nstu_list = %d, context.students_list = %d", students_list, Context.students_list);
    //display_list(students_list, subjects, N);
    
    fclose(fp);
    return 1;
}


void strcpy(char *source, char *dest){
    int i = 0;
    
    for(i = 0; source[i] != '\0' ; i++){
        dest[i] = source[i];
    }
    dest[i] = '\0';
}

void get_marks_student_subject(char *s_name, struct students_head *students_list, subject_name *subjects, int N){
    int i;
    int sub_code;
    struct student_node *walker = students_list-> first;
    
    printf("\nSubject Codes:");
    for(i = 0 ; i < N; i++){
        printf("\n%s : %d", subjects[i].name, i);
    }
    printf("\nEnter the code of the subject u would like to know:");
    scanf("%d", &sub_code);
    if((sub_code < 0) || (sub_code >= N)){
        printf("\nInvalid subject code.");
        return;
    }
    
    for( ; walker ; walker = walker-> next){
        if (str_equal(s_name, walker-> name)){
            break;
        }
    }
    
    if(walker == NULL){
        printf("\nNo student of the name %s\n", s_name);
        return;
    }
    else{
        printf("\nMarks obtained by %s in %s is : %d", s_name, subjects[sub_code].name, walker-> marks[sub_code]);
    }
    
    return;
}

void get_max_marks_subject_student(struct students_head *students_list, subject_name *subjects, int N){
    int i;
    int sub_code;
    struct student_node *walker = students_list-> first;
    struct student_node *topper = NULL;
    int max_marks = 0;
    
   printf("\nSubject Codes:");
    for(i = 0 ; i < N; i++){
        printf("\n%s : %d", subjects[i].name, i);
    }
    printf("\nEnter the code of the subject u would like to know:");
    scanf("%d", &sub_code);
    if((sub_code < 0) || (sub_code >= N)){
        printf("\nInvalid subject code.");
        return;
    }
    
    for( ; walker ; walker = walker-> next){
        if(walker-> marks[sub_code] > max_marks){
            max_marks = walker-> marks[sub_code];
            topper = walker;
        }
    }
    
    printf("Topper of %s is %s with a score of %d", subjects[sub_code].name, topper-> name, max_marks);
    
    return;
}

void get_marks_subject(struct students_head *students_list, subject_name *subjects, int N){
    int sub_code;
    int i;
    struct student_node *walker = students_list-> first;
    
    printf("\nSubject Codes:");
    for(i = 0 ; i < N; i++){
        printf("\n%s : %d", subjects[i].name, i);
    }
    printf("\nEnter the code of the subject u would like to know:");
    scanf("%d", &sub_code);
    if((sub_code < 0) || (sub_code >= N)){
        printf("\nInvalid subject code.");
        return;
    }
    
    printf("%15s: %6s", subjects[sub_code].name, "Marks");
    while(walker){
        printf("\n%40s: %6d", walker-> name, walker-> marks[sub_code]);
        walker = walker-> next;
    }
    
    return;
}

void get_min_marks_student(char *s_name, struct students_head *students_list, subject_name *subjects, int N){
    struct student_node *walker = students_list-> first;
    int i ;
    
    for( ; walker ; walker = walker-> next){
        if (str_equal(s_name, walker-> name)){
            break;
        }
    }
    
    if(walker == NULL){
        printf("\nNo student of the name %s\n", s_name);
        return;
    }
    else{
        int min_index = 0;
        for(i = 0 ; i < N ; i++){
            if(walker-> marks[i] < walker-> marks[min_index]){
                min_index = i;
            }
        }
        printf("\n%s obtained min marks in %s, with a score of %d", s_name, subjects[min_index].name, walker->marks[min_index]);
        printf("\n");
    }
}

void get_max_marks_student(char *s_name, struct students_head *students_list, subject_name *subjects, int N){
    struct student_node *walker = students_list-> first;
    int i ;
    
    for( ; walker ; walker = walker-> next){
        if (str_equal(s_name, walker-> name)){
            break;
        }
    }
    
    if(walker == NULL){
        printf("\nNo student of the name %s\n", s_name);
        return;
    }
    else{
        int max_index = 0;
        for(i = 0 ; i < N ; i++){
            if(walker-> marks[i] > walker-> marks[max_index]){
                max_index = i;
            }
        }
        printf("\n%s obtained max marks in %s, with a score of %d", s_name, subjects[max_index].name, walker->marks[max_index]);
        printf("\n");
    }
}

void get_all_marks_student(char *s_name, struct students_head *students_list,subject_name *subjects, int N){
    struct student_node *walker = students_list-> first;
    int i ;
    
    for( ; walker ; walker = walker-> next){
        if (str_equal(s_name, walker-> name)){
            break;
        }
    }
    
    if(walker == NULL){
        printf("\nNo student of the name %s\n", s_name);
        return;
    }
    else{
        for(i = 0 ; i < N ; i++){
            printf("\n%s: %d", subjects[i].name, walker-> marks[i]);
        }
        printf("\n");
    }
    
}

void add_student(){
    char s_name[40];
    int marks[Context.N];
    int i;
    
    printf("\nEnter name of the student: ");
    scanf("%s", s_name);
    if(name_already_exists(s_name)){
        printf("\nStudent details already exists");
        return;
    }
    printf("\nEnter the marks obtained in the respective subjects:");
    for(i = 0 ; i < Context.N ; i++){
        printf("\n%s: ", Context.subjects[i].name);
        scanf("%d", &marks[i]);
    }
    
    insert(Context.students_list, s_name, marks, Context.N);
    if(! Context.flag_append_list_present){
        Context.append_list = Context.students_list-> last;
        Context.flag_append_list_present = 1;
    }
    
    return;
}

int name_already_exists(char *s_name){
    struct student_node *walker = Context.students_list-> first;
    
    for( ; walker ; walker = walker-> next){
        if(str_equal(walker-> name, s_name)){
            return 1;
        }
    }
    return 0;
    
}

void get_average_marks_student(char *s_name, struct students_head *students_list, struct subject_name *subjects, int N){
    struct student_node *walker = students_list-> first;
    int i ;
    int sum = 0;
    float avg;
    
    for( ; walker ; walker = walker-> next){
        if (str_equal(s_name, walker-> name)){
            break;
        }
    }
    
    if(walker == NULL){
        printf("\nNo student of the name %s\n", s_name);
        return;
    }
    else{
        for(i = 0 ; i < N ; i++){
            sum += walker-> marks[i];
        }
        printf("\n");
    }
    avg = (float)sum / N;
    printf("\nAverage of %s = %f", s_name, avg);
    return;
}

void save_and_exit(){
    FILE *fp = NULL;
    
    if(!(fp = fopen(Context.file_name, "a"))){
        printf("Error opening file to append");
        return;
    }
    else{ 
        int i;
        
        if(Context.flag_append_list_present){
        struct student_node *reader = Context.append_list;
        
        for( ; reader ; reader = reader-> next){
            fprintf(fp, "%s\r\n", reader-> name);
            for(i = 0 ; i < Context.N - 1 ; i++){
                fprintf(fp, "%d,", reader-> marks[i]);
            }
            fprintf(fp, "%d", reader-> marks[i]);
            fprintf(fp, "\r\n");
        }
        }
        
        fclose(fp);
    }
    return;
}

int str_equal(char *s1, char *s2){
    int i ;
    
    for(i = 0; s1[i] != '\0' ; i++){
        if(s1[i] != s2[i]){
            return 0;
        }
    }
    if(s2[i] == '\0'){
        return 1;
    }
}

int is_digit(char c){
    if (c <= '9' & c >= '0'){
        return 1;
    }
    return 0;
}