#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#define MAX_STUDENTS 300
#define MAX_GRADES 10
#define N 10

typedef struct {
    char *id;
    int R;
    char *lastName;
    char *firstName;
    float *grades;
} Student;

int numStudents = 0;
Student *students;
FILE *file;

void Menu();
void deleteAllStudent();
void addStudent();
void displayStudent(char id[11]);
void displayAllStudents();
void about_us();
void modifyStudent(char id[11]);
void deleteStudent(char id[11]);

int main() {
    file = fopen("etudiant.txt", "r+");
    if (file == NULL) {
        printf("\033[1;31mError: Could not open file\033[0m\n");
        exit(1);
    }

    char line[1024];
while (fgets(line, sizeof(line), file) != NULL) {
    int R,i,j;
    char id[12];
    char lastName[52];
    char firstName[52];
    float grades[MAX_GRADES]; 

    int result = sscanf(line, "Student_Id: %[^,],Last_Name: %[^,],First_Name: %[^,],Number_Of_Grades: %d,Grades: %f,%f,%f,%f,%f,%f,%f,%f",
    id, lastName, firstName, &R, &grades[0], &grades[1], &grades[2], &grades[3], &grades[4], &grades[5], &grades[6], &grades[7]);

    if (result != 11 - (7 - R)) {
        printf("\033[1;31mError: Invalid file format\033[0m\n");
        exit(1);
    }

         students=(Student*)realloc(students,(numStudents+1)*sizeof(Student));
        (students+numStudents)->id=(char*)malloc(strlen(id) + 1); 
        (students+numStudents)->lastName=(char*)malloc(strlen(lastName) + 1); 
        (students+numStudents)->firstName=(char*)malloc(strlen(firstName) + 1); 
        (students+numStudents)->grades=(float*)malloc(R * sizeof(float)); 
        strcpy(students[numStudents].id, id);
        (students+numStudents)->R = R;
        strcpy(students[numStudents].lastName, lastName);
        strcpy(students[numStudents].firstName, firstName);
        memcpy(students[numStudents].grades, grades, sizeof(float) * (students+numStudents)->R);
        numStudents++;
    }

    fclose(file);

    Menu();

    file = fopen("etudiant.txt", "w");
    if (file == NULL) {
        printf("\033[1;31mError: Could not open file\033[0m\n");
        exit(1);
    }

    for (i = 0; i < numStudents; i++) {
        fprintf(file, "Student_Id: %s,Last_Name: %s,First_Name: %s,Number_Of_Grades: %d,Grades: ",students[i].id, students[i].lastName, students[i].firstName, students[i].R);
        for (j=0; j < students[i].R - 1; j++){
        	fprintf(file, "%.2f,", students[i].grades[j]);
        }
        fprintf(file, "%.2f\n", students[i].grades[students[i].R - 1]);
    }
    
    fclose(file);
    for(i=0;i<numStudents;i++){
	
    free((students+i)->firstName);
    free((students+i)->lastName);
    free((students+i)->grades);
    (students+i)->firstName=NULL;
    (students+i)->lastName=NULL;
    (students+i)->grades=NULL;
    
}
    free(students);
    return 0;
}


void Menu() {
int i,choice;
char id[11];
while(1) {
    printf("\n\t\t\t\033[1;34m-------------------------------------------------\n");
    printf("\t\t\t-"); printf("       What would you like to do?\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  1. Add a student         \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  2. Search for a student     \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  3. Display all students  \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  4. Modify a student      \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  5. Delete a student      \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  6. about us               \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  7. delete all students   \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  8. clear the consol        \t\t\t");printf("-\n");
    printf("\t\t\t-"); printf("  9. Exit                  \t\t\t");printf("-");
    printf("\n\t\t\t-------------------------------------------------\033[0m\n");
    printf("\n\t\t==>Enter your choice : ");
    scanf("%d",&choice);

    switch (choice) {
        case 1:
            addStudent();
            break;
        case 2:
            printf("Enter the ID of the student: ");
            scanf("%s", id);
            displayStudent(id);
            break;
        case 3:
            displayAllStudents();
            break;
        case 4:
            printf("Enter the ID of the student: ");
            scanf("%s", id);
            modifyStudent(id);
            break;
        case 5:
            printf("Enter the ID of the student: ");
            scanf("%s", id);
            deleteStudent(id);
            break;
        case 6:
		       about_us();
		       break;
		case 7:
		       deleteAllStudent();
			   break; 	       
        case 8:
		       system("cls");
			   break;    
        case 9:
        	printf("\nExiting program..");
        	for(i=0;i<4;i++){
                printf(".");
                sleep(1);
	        return;
        default:
            printf("\033[1;31mError: Invalid choice\033[0m\n");
            break;
    }
    printf("\nredirected to menu..");
    for(i=0;i<4;i++){
    	printf(".");
    	sleep(1);
	}
 
}

}

void addStudent() {
    int i;
    if (numStudents == MAX_STUDENTS) {
        printf("\033[1;31mError: Maximum number of students reached\033[0m\n");
        return;
    }
    Student newStudent;
    newStudent.id = (char*)malloc(12); 
    newStudent.lastName = (char*)malloc(21); 
    newStudent.firstName = (char*)malloc(16); 
    newStudent.grades = (float*)malloc(10 * sizeof(float)); 

    fflush(stdin);
    printf("==>Enter the Massar Id : ");
    fgets(newStudent.id, 12, stdin); 
    newStudent.id[strcspn(newStudent.id, "\n")] = '\0';

    for (i = 0; i < numStudents; i++) {
        if (strcmp(students[i].id, newStudent.id) == 0) {
            printf("\n\033[1;31mERROR:This Id is used by other student !!\033[0m\n");
            while (strcmp(students[i].id, newStudent.id) == 0) {
                printf("==>Enter A New Massar Id : ");
                fgets(newStudent.id, 12, stdin);
                newStudent.id[strcspn(newStudent.id, "\n")] = '\0';
                if (strcmp(students[i].id, newStudent.id) == 0) {
                    printf("\n\033[1;31mERROR:This Id is used by other student !!\033[0m\n");
                    printf("==>Enter A New Massar Id : ");
                    fgets(newStudent.id, 12, stdin);
                    newStudent.id[strcspn(newStudent.id, "\n")] = '\0';
                }
            }
        }
    }

    printf("==>Enter the last name : ");
    fgets(newStudent.lastName, 21, stdin);
    newStudent.lastName[strcspn(newStudent.lastName, "\n")] = '\0';

    printf("==>Enter the first name : ");
    fgets(newStudent.firstName, 16, stdin);
    newStudent.firstName[strcspn(newStudent.firstName, "\n")] = '\0';

    fflush(stdin);
    printf("==>Enter number of tests you want to enter (Max 10): ");
    scanf("%d", &newStudent.R);

    printf("==>Enter the %d grades of the student separated by spaces: ", newStudent.R);
    for (i = 0; i < newStudent.R; i++) {
        scanf("%f", &newStudent.grades[i]);
    }

    students = (Student*)realloc(students, (numStudents + 1) * sizeof(Student));

    (students + numStudents)->id = (char*)malloc(strlen(newStudent.id) + 1); 
    (students + numStudents)->lastName = (char*)malloc(strlen(newStudent.lastName) + 1); 
    (students + numStudents)->firstName = (char*)malloc(strlen(newStudent.firstName) + 1); 
    (students + numStudents)->grades = (float*)malloc(newStudent.R * sizeof(float));
    strcpy(students[numStudents].id, newStudent.id);
    (students + numStudents)->R = newStudent.R;
    strcpy(students[numStudents].lastName, newStudent.lastName);
    strcpy(students[numStudents].firstName, newStudent.firstName);
    memcpy(students[numStudents].grades, newStudent.grades, sizeof(float) * (students + numStudents)->R);

    numStudents++;

    printf("Student added successfully!\n");
    free(newStudent.id);
    free(newStudent.lastName);
    free(newStudent.firstName);
    free(newStudent.grades);
}
void displayStudent(char id[11]) {
    int i,j;
    for (i = 0; i < numStudents; i++) {
        if (strcasecmp(students[i].id, id) == 0) {
            printf("\n\033[1;36m-------------------------------------------Student Details------------------------------------------------\n");
            printf("\tId Massar : %s\n", students[i].id);
            printf("\tLast Name : %s\n", students[i].lastName);
            printf("\tFirst Name : %s\n", students[i].firstName);
            printf("\tNumber of Grades : %d\n", students[i].R);
            printf("\tGrades :");
            for (j = 0; j < students[i].R; j++) {
                printf(" %.2f", students[i].grades[j]);
            }
            printf("\n-----------------------------------------------------------------------------------------------------------\033[1;36m\n");
            printf("\n");
      return;
        }
    }
    printf("\033[1;31mError: Student not found\033[0m\n");
    return;

}
void displayAllStudents() {
	int i,j;
    if (numStudents == 0) {
        printf("\nNo students to display.\n");
        return;
    }
    printf("\n\033[1;36m-------------------------------------------Student Details------------------------------------------------\n");
    for (i = 0; i < numStudents; i++) {
       printf("student %d",i+1); printf("\t");printf("\tId Massar : %s\n", students[i].id);
                                 printf("\t\t");printf("\tLast Name : %s\n", students[i].lastName);
                                 printf("\t\t");printf("\tFirst Name : %s\n", students[i].firstName);
                                 printf("\t\t");printf("\tNumber of Grades : %d\n", students[i].R);
                                 printf("\t\t");printf("\tGrades :");
        for (j = 0; j < students[i].R; j++) {
            printf(" %.2f", students[i].grades[j]);
        }
        printf("\n-----------------------------------------------------------------------------------------------------------\033[1;36m\n");
    }
}

void modifyStudent(char id[11]) {
int i,j,choice;
for (i = 0; i < numStudents; i++) {	
char lastName[20];
char firstName[20];
int grades[MAX_GRADES];
if (strcmp(students[i].id, id) == 0) {
            printf("\n---------------------Student Details-----------------------\n");
            printf("\tId Massar : %s\n", students[i].id);
            printf("\tLast Name : %s\n", students[i].lastName);
            printf("\tFirst Name : %s\n", students[i].firstName);
            printf("\tNumber of Grades : %d\n", students[i].R);
            printf("\tGrades :");
            for (j = 0; j < students[i].R-1; j++) {
                printf(" %.2f ,", students[i].grades[j]);
            }
                 printf(" %.2f", students[i].grades[students[i].R-1]);
            printf("\n------------------------------------------------------------\n");
        	
    while(1){
	    printf("\n\t\t\t\033[1;31m------------------------------------------------------------\n");
            printf("\t\t\t-");printf("\t\tWhat would you like to modify?\t\t ");printf("  -\n");
            printf("\t\t\t-");printf("\t\t1. Last Name    \t\t\t");printf("   -\n");
            printf("\t\t\t-");printf("\t\t2. First Name     \t\t        ");printf("   -\n");
            printf("\t\t\t-");printf("\t\t3. Number of Grades and Grades\t\t");printf("   -\n");
            printf("\t\t\t-");printf("\t\t4. Save and return to menu\t\t ");printf("  -");
            printf("\n\t\t\t------------------------------------------------------------\033[0m\n");
            printf("\t\t==>Enter your choice: ");
            scanf("%d", &choice);
             switch (choice) {
                case 1:
                    printf("\nEnter new Last Name: ");
                    fflush(stdin);
                    fgets(lastName,20,stdin);
                    lastName[strcspn(lastName, "\n")] = '\0';
                    students[i].lastName=(char*)realloc(students[i].lastName,strlen(lastName));
                    strcpy(students[i].lastName,lastName);
                    printf("Student last name with ID %s has been modified in the records.\n", id);
                    break;
                case 2:
                    printf("\nEnter new First Name: ");
                    fflush(stdin);
                    fgets(firstName,20,stdin);
                    firstName[strcspn(firstName, "\n")] = '\0';
                    students[i].firstName=(char*)realloc(students[i].firstName,strlen(firstName));
                    strcpy(students[i].firstName,firstName);
                    printf("Student last name with ID %s has been modified in the records.\n", id);
                    break;
                case 3:
                    printf("\nEnter new Number of Grades: ");
                    fflush(stdin);
                    scanf("%d", &students[i].R);
                    printf("Number of grades for student with ID %s has been modified in the records.\n", id);
                    printf("Enter new Grades (separated by spaces): ");
                    for (j = 0; j < students[i].R; j++) {
                        scanf("%f,", &grades[j]);
                    }
                    students[i].grades=(float*)realloc(students[i].grades,students[i].R*sizeof(float));
                    memcpy((students+i)->grades, grades, sizeof(float) * (students + i)->R);
                    printf("Grades for student with ID %s have been modified in the records.\n", id);
                    break;

                case 4:
                    printf("Student modified successfully!\n");
                    return;
                default:
                    printf("\nInvalid input. Please enter a value between 1 and 4.\n");
                    break;
                }
			}
		}
	}
	printf("\033[1;31mError: Student not found\033[0m\n");
}

void deleteStudent(char id[11]) {
int i,j;
for (i = 0; i < numStudents; i++) {
if (strcmp(students[i].id, id) == 0) {
for (j = i; j < numStudents - 1; j++) {
students[j] = students[j + 1];
}
numStudents--;
        printf("press any key.....\n");
        fflush(stdin);
        getchar();
        printf("Student deleted successfully!\n");
        return;
    }
}

printf("\033[1;31mError: Student not found\033[0m\n");
}
void deleteAllStudent() {
	int i,j;
	char verify[3];
	if(numStudents==0){
		printf("\n\t\t there are no students to delete .\n");
		return;
	}
	else{
	
	printf("\n\t\t are you sure you want to delete all students (yes \\ no) :");
	fflush(stdin);
	scanf("%s",verify);
	strlwr(verify);
	if(strcmp(verify,"yes")==0){
    for (i = 0; i < numStudents; i++) {

	
    for (j = numStudents-1; j >= i; j--) {
    students[j] = students[i-j];
    numStudents--;
           }
    }
    printf("press any key.....\n");
    fflush(stdin);
    getchar();
    printf("Students deleted successfully!\n");
    return;
}
    else
    return;
    
}
}
void about_us(){
	int i;
	FILE *p=fopen("about us.txt","r");
	printf("\n=========================================================================================\n");
	while((i=getc(p))!=EOF){
		putchar(i);
	}
	printf("\n=========================================================================================\n");
}

