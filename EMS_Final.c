#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMPLOYEES 100
#define MAX_ATTENDANCE 500
#define MAX_STRING 50
#define MAX_USERS 10
#define MAX_PASSWORD 20

typedef struct
{
    char username[MAX_STRING];
    char password[MAX_PASSWORD];
    char role[MAX_STRING]; // "HR" or "ADMIN"
    int is_active;
} User;

// Global variables for authentication
User users[MAX_USERS];
int user_count = 0;
User *current_user = NULL;
int is_logged_in = 0;

typedef struct
{
    int emp_id;
    char name[MAX_STRING];
    char position[MAX_STRING];
    float hourly_salary;
    int is_active;
} Employee;

// Attendance {Shams}
typedef struct
{
    int emp_id;
    char date[15];
    float hours_worked;
    int present;
    int excused;
} Attendance;

Employee employees[MAX_EMPLOYEES];
Attendance attendance_records[MAX_ATTENDANCE];
int employee_count = 0;
int attendance_count = 0;

// Attendance End

// Authentication functions{Moballig}
int authenticate_user();
void login_menu();
void change_password();
void logout();
int has_permission(const char *required_role);
void save_users_to_file();
void load_users_from_file();

// Main system functions
void show_menu();             
void add_employee();          
void view_employees();        
void update_employee();       
void delete_employee();       
void mark_daily_attendance(); 
void view_attendance();       
void calculate_payroll();     
void save_to_file();          
void load_from_file();        
int get_next_id();            
void pause();                 
void clear_screen();          

int main() 
{

    load_users_from_file();
    load_from_file();

    while (1)
    {
        if (!is_logged_in)
        {
            login_menu();
            if (!is_logged_in)
            {
                printf("\nExiting system...\n");
                break;
            }
        }
        else
        {
            int choice;
            do
            {
                show_menu();
                printf("\nEnter your choice: ");
                scanf("%d", &choice);

                switch (choice)
                {
                case 1:
                    clear_screen();
                    if (has_permission("HR"))
                    {
                        add_employee();
                    }
                    else
                    {
                        printf("\nAccess denied! Only HR can add employees.\n");
                    }
                    break;
                case 2:
                    clear_screen();
                    view_employees();
                    break;
                case 3:
                    clear_screen();
                    if (has_permission("HR") || has_permission("ADMIN"))
                    {
                        update_employee();
                    }
                    else
                    {
                        printf("\nAccess denied!\n");
                    }
                    break;
                case 4:
                    clear_screen();
                    if (has_permission("HR"))
                    {
                        delete_employee();
                    }
                    else
                    {
                        printf("\nAccess denied! Only HR can delete employees.\n");
                    }
                    break;
                case 5:
                    clear_screen();
                    if (has_permission("HR"))
                    {
                        mark_daily_attendance();
                    }
                    else
                    {
                        printf("\nAccess denied! Only HR can mark attendance.\n");
                    }
                    break;
                case 6:
                    clear_screen();
                    view_attendance();
                    break;
                case 7:
                    clear_screen();
                    if (has_permission("HR"))
                    {
                        calculate_payroll();
                    }
                    else
                    {
                        printf("\nAccess denied! Only HR can calculate payroll.\n");
                    }
                    break;
                case 8:
                    clear_screen();
                    change_password();
                    break;
                case 9:
                    logout();
                    break;
                case 10:
                    clear_screen();
                    save_to_file();
                    save_users_to_file();
                    printf("\n========================================\n");
                    printf("Data saved successfully!\n");
                    printf("Thank you for using Employee\n");
                    printf("Management System!\n");
                    printf("========================================\n");
                    return 0;
                default:
                    printf("\nInvalid choice! Please try again.\n");
                }

                if (choice != 8 && choice != 9 && choice != 10)
                {
                    pause();
                    clear_screen();
                }
            } while (choice != 8 && choice != 9 && choice != 10);
        }
    }

    return 0;
}

void show_menu()
{
    clear_screen();

    printf("\n");
    printf("    ███████╗███╗   ███╗██████╗ ██╗      ██████╗ ██╗   ██╗███████╗███████╗    ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗ \n");
    printf("    ██╔════╝████╗ ████║██╔══██╗██║     ██╔═══██╗╚██╗ ██╔╝██╔════╝██╔════╝    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗\n");
    printf("    █████╗  ██╔████╔██║██████╔╝██║     ██║   ██║ ╚████╔╝ █████╗  █████╗      ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝\n");
    printf("    ██╔══╝  ██║╚██╔╝██║██╔═══╝ ██║     ██║   ██║  ╚██╔╝  ██╔══╝  ██╔══╝      ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗\n");
    printf("    ███████╗██║ ╚═╝ ██║██║     ███████╗╚██████╔╝   ██║   ███████╗███████╗    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║\n");
    printf("    ╚══════╝╚═╝     ╚═╝╚═╝     ╚══════╝ ╚═════╝    ╚═╝   ╚══════╝╚══════╝    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
    printf("\n");

    if (current_user && is_logged_in)
    {
        printf("                                    ╔═════════════════════════════════════════════════════════════════════╗\n");
        printf("                                    ║  Logged in as: %-15s  ║ ║ Role: %-10s                ║\n",
               current_user->username, current_user->role);
        printf("                                    ╚═════════════════════════════════════════════════════════════════════╝\n");
    }

    printf("╔═════════════════════════════════╗ ╔═════════════════════════════════╗ ╔═════════════════════════════════╗ ╔═════════════════════════════════╗\n");
    printf("║  1. Add New Employee            ║ ║  2. View All Employees          ║ ║  3. Update Employee Details     ║ ║  4. Delete Employee             ║\n");
    printf("╚═════════════════════════════════╝ ╚═════════════════════════════════╝ ╚═════════════════════════════════╝ ╚═════════════════════════════════╝\n");

    printf("╔═════════════════════════════════╗ ╔═════════════════════════════════╗ ╔═════════════════════════════════╗ ╔═════════════════════════════════╗\n");
    printf("║  5. Mark Daily Attendance       ║ ║  6. View Attendance Records     ║ ║  7. Calculate Payroll           ║ ║  8. Change Password             ║\n");
    printf("╚═════════════════════════════════╝ ╚═════════════════════════════════╝ ╚═════════════════════════════════╝ ╚═════════════════════════════════╝\n");

    printf("                                    ╔═════════════════════════════════╗ ╔═════════════════════════════════╗\n");
    printf("                                    ║  9. Logout                      ║ ║  10. Save & Exit                ║\n");
    printf("                                    ╚═════════════════════════════════╝ ╚═════════════════════════════════╝\n");
}

void add_employee()
{
    if (employee_count >= MAX_EMPLOYEES)
    {
        printf("\nCannot add more employees. Maximum limit reached!\n");
        return;
    }

    Employee new_emp;
    new_emp.emp_id = get_next_id();
    new_emp.is_active = 1;

    printf("\n============================================\n");
    printf("           ADD NEW EMPLOYEE                 \n");
    printf("============================================\n");
    printf("Employee ID: %03d\n", new_emp.emp_id);

    printf("Enter Employee Name: ");
    getchar();
    fgets(new_emp.name, MAX_STRING, stdin);
    new_emp.name[strcspn(new_emp.name, "\n")] = 0;

    printf("Enter Position: ");
    fgets(new_emp.position, MAX_STRING, stdin);
    new_emp.position[strcspn(new_emp.position, "\n")] = 0;

    printf("Enter Hourly Salary Rate: $");
    scanf("%f", &new_emp.hourly_salary);

    employees[employee_count] = new_emp;
    employee_count++;

    printf("\nEmployee added successfully!\n");
    printf("Employee ID: %03d\n", new_emp.emp_id);
    printf("Name: %s\n", new_emp.name);
    printf("Position: %s\n", new_emp.position);
    printf("Hourly Rate: $%.2f/hour\n", new_emp.hourly_salary);
}

void view_employees()
{
    printf("\n============================================\n");
    printf("             ALL EMPLOYEES                  \n");
    printf("============================================\n");
    if (employee_count == 0)
    {
        printf("No employees found!\n");
        return;
    }

    printf("%-5s %-25s %-20s %-15s %-8s\n",
           "ID", "Name", "Position", "Hourly Rate", "Status");
    printf("---------------------------------------------------------------------------\n");

    int active_count = 0;
    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].is_active)
        {
            printf("%-5.3d %-25s %-20s $%-14.2f %-8s\n",
                   employees[i].emp_id,
                   employees[i].name,
                   employees[i].position,
                   employees[i].hourly_salary,
                   "Active");
            active_count++;
        }
        if (!employees[i].is_active)
        {
            printf("%-5.3d %-25s %-20s $%-14.2f %-8s\n",
                   employees[i].emp_id,
                   employees[i].name,
                   employees[i].position,
                   employees[i].hourly_salary,
                   "Inctive");
        }
    }
    printf("---------------------------------------------------------------------------\n");
    printf("\nTotal Active Employees: %d\n", active_count);
}

void update_employee()
{
    int emp_id, found = 0;
    printf("\n============================================\n");
    printf("             UPDATE EMPLOYEE               \n");
    printf("============================================\n");
    printf("Enter Employee ID to update: ");
    scanf("%d", &emp_id);

    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].emp_id == emp_id && employees[i].is_active)
        {
            found = 1;
            printf("\nCurrent Details:\n");
            printf("--------------------\n");
            printf("ID: %03d\n", employees[i].emp_id);
            printf("Name: %s\n", employees[i].name);
            printf("Position: %s\n", employees[i].position);
            printf("Hourly Rate: $%.2f/hour\n", employees[i].hourly_salary);
            printf("--------------------\n");

            printf("\nEnter new details:\n");
            printf("New Name: ");
            getchar();
            fgets(employees[i].name, MAX_STRING, stdin);
            employees[i].name[strcspn(employees[i].name, "\n")] = 0;

            printf("New Position: ");
            fgets(employees[i].position, MAX_STRING, stdin);
            employees[i].position[strcspn(employees[i].position, "\n")] = 0;

            printf("New Hourly Rate: $");
            scanf("%f", &employees[i].hourly_salary);

            printf("\nEmployee updated successfully!\n");
            break;
        }
    }

    if (!found)
    {
        printf("\nEmployee with ID %d not found or inactive!\n", emp_id);
    }
}

void delete_employee()
{
    int emp_id, found = 0;
    printf("\n============================================\n");
    printf("             DELETE EMPLOYEE               \n");
    printf("============================================\n");
    printf("Enter Employee ID to delete: ");
    scanf("%d", &emp_id);

    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].emp_id == emp_id && employees[i].is_active)
        {
            found = 1;
            printf("\nEmployee to delete:\n");
            printf("ID: %03d, Name: %s, Position: %s\n",
                   employees[i].emp_id, employees[i].name, employees[i].position);

            char confirm;
            printf("Are you sure you want to delete this employee? (y/n): ");
            scanf(" %c", &confirm);

            if (confirm == 'y' || confirm == 'Y')
            {
                employees[i].is_active = 0;
                printf("Employee deleted successfully!\n");
            }
            else
            {
                printf("Delete operation cancelled.\n");
            }
            break;
        }
    }

    if (!found)
    {
        printf("\nEmployee with ID %d not found or inactive!\n", emp_id);
    }
}

void view_attendance()
{
    printf("\n============================================\n");
    printf("           ATTENDANCE RECORDS              \n");
    printf("============================================\n");
    if (attendance_count == 0)
    {
        printf("No attendance records found!\n");
        return;
    }

    printf("%-8s %-25s %-12s %-8s %-10s %-15s\n",
           "Emp ID", "Employee Name", "Date", "Hours", "Status", "Absence Type");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < attendance_count; i++)
    {
        char emp_name[MAX_STRING] = "Unknown";
        for (int j = 0; j < employee_count; j++)
        {
            if (employees[j].emp_id == attendance_records[i].emp_id)
            {
                strcpy(emp_name, employees[j].name);
                break;
            }
        }

        printf("%-8.3d %-25s %-12s %-8.2f %-10s",
               attendance_records[i].emp_id,
               emp_name,
               attendance_records[i].date,
               attendance_records[i].hours_worked,
               attendance_records[i].present ? "Present" : "Absent");

        if (!attendance_records[i].present)
        {
            printf(" %-15s", attendance_records[i].excused ? "Excused" : "Unexcused");
        }
        else
        {
            printf(" %-15s", "N/A");
        }
        printf("\n");
    }
    printf("--------------------------------------------------------------------------------\n");
}

void save_to_file()
{
    FILE *emp_file = fopen("employees.txt", "w");
    if (emp_file)
    {
        fprintf(emp_file, "Employee Management System - Employee Records\n");
        fprintf(emp_file, "==============================================\n");
        fprintf(emp_file, "Total Employees: %d\n\n", employee_count);

        fprintf(emp_file, "%-5s %-25s %-20s %-15s %-8s\n",
                "ID", "Name", "Position", "Hourly Rate", "Status");
        fprintf(emp_file, "-----------------------------------------------------------------------\n");

        for (int i = 0; i < employee_count; i++)
        {
            fprintf(emp_file, "%-5.3d %-25s %-20s $%-14.2f %-8s\n",
                    employees[i].emp_id,
                    employees[i].name,
                    employees[i].position,
                    employees[i].hourly_salary,
                    employees[i].is_active ? "Active" : "Inactive");
        }
        fprintf(emp_file, "-----------------------------------------------------------------------\n");

        fprintf(emp_file, "\n\nRaw Data (for system loading):\n");
        fprintf(emp_file, "%d\n", employee_count);
        for (int i = 0; i < employee_count; i++)
        {
            fprintf(emp_file, "%03d|%s|%s|%.2f|%d\n",
                    employees[i].emp_id,
                    employees[i].name,
                    employees[i].position,
                    employees[i].hourly_salary,
                    employees[i].is_active);
        }
        fclose(emp_file);
    }

    FILE *att_file = fopen("attendance.txt", "w");
    if (att_file)
    {
        fprintf(att_file, "Employee Management System - Attendance Records\n");
        fprintf(att_file, "================================================\n");
        fprintf(att_file, "Total Records: %d\n\n", attendance_count);

        fprintf(att_file, "%-8s %-25s %-12s %-8s %-10s %-15s\n",
                "Emp ID", "Employee Name", "Date", "Hours", "Status", "Absence Type");
        fprintf(att_file, "--------------------------------------------------------------------------------\n");

        for (int i = 0; i < attendance_count; i++)
        {
            char emp_name[MAX_STRING] = "Unknown";
            for (int j = 0; j < employee_count; j++)
            {
                if (employees[j].emp_id == attendance_records[i].emp_id)
                {
                    strcpy(emp_name, employees[j].name);
                    break;
                }
            }

            fprintf(att_file, "%-8.3d %-25s %-12s %-8.2f %-10s %-15s\n",
                    attendance_records[i].emp_id,
                    emp_name,
                    attendance_records[i].date,
                    attendance_records[i].hours_worked,
                    attendance_records[i].present ? "Present" : "Absent",
                    !attendance_records[i].present ? (attendance_records[i].excused ? "Excused" : "Unexcused") : "N/A");
        }
        fprintf(att_file, "--------------------------------------------------------------------------------\n");

        fprintf(att_file, "\n\nRaw Data (for system loading):\n");
        fprintf(att_file, "%d\n", attendance_count);
        for (int i = 0; i < attendance_count; i++)
        {
            fprintf(att_file, "%03d|%s|%.2f|%d|%d\n",
                    attendance_records[i].emp_id,
                    attendance_records[i].date,
                    attendance_records[i].hours_worked,
                    attendance_records[i].present,
                    attendance_records[i].excused);
        }
        fclose(att_file);
    }
    // Save payroll data
    FILE *payroll_file = fopen("payroll_report.txt", "w");
    if (payroll_file)
    {
        fprintf(payroll_file, "Employee Management System - Payroll Report\n");
        fprintf(payroll_file, "============================================\n");

        // Find all unique months from attendance records
        char months[100][8];
        int month_count = 0;

        for (int i = 0; i < attendance_count; i++)
        {
            char record_month[8];
            strcpy(record_month, &attendance_records[i].date[3]);

            int found = 0;
            for (int j = 0; j < month_count; j++)
            {
                if (strcmp(months[j], record_month) == 0)
                {
                    found = 1;
                    break;
                }
            }

            if (!found && month_count < 100)
            {
                strcpy(months[month_count], record_month);
                month_count++;
            }
        }

        // Generate payroll for each month
        for (int m = 0; m < month_count; m++)
        {
            fprintf(payroll_file, "\n============================================\n");
            fprintf(payroll_file, "           PAYROLL FOR %s             \n", months[m]);
            fprintf(payroll_file, "============================================\n");
            fprintf(payroll_file, "%-5s %-25s %-12s %-6s %-10s %-12s %-12s %-12s %-12s\n",
                    "ID", "Name", "Hourly-Rate", "Days", "Reg Hours", "Overtime-Pay", "Penalties", "Gross-Pay", "Net-Pay");
            fprintf(payroll_file, "------------------------------------------------------------------------------------------------------------\n");

            float total_payroll = 0;

            for (int i = 0; i < employee_count; i++)
            {
                if (employees[i].is_active)
                {
                    float total_hours = 0;
                    int present_days = 0;
                    int unexcused_absences = 0;

                    for (int j = 0; j < attendance_count; j++)
                    {
                        if (attendance_records[j].emp_id == employees[i].emp_id)
                        {
                            char record_month[8];
                            strcpy(record_month, &attendance_records[j].date[3]);

                            if (strcmp(record_month, months[m]) == 0)
                            {
                                if (attendance_records[j].present)
                                {
                                    present_days++;
                                    total_hours += attendance_records[j].hours_worked;
                                }
                                else if (!attendance_records[j].excused)
                                {
                                    unexcused_absences++;
                                }
                            }
                        }
                    }

                    if (present_days > 0 || unexcused_absences > 0)
                    {
                        float standard_hours = present_days * 8.0;
                        float regular_hours = (total_hours <= standard_hours) ? total_hours : standard_hours;
                        float overtime_hours = (total_hours > standard_hours) ? (total_hours - standard_hours) : 0;

                        float regular_pay = regular_hours * employees[i].hourly_salary;
                        float overtime_pay = overtime_hours * employees[i].hourly_salary * 1.5;
                        float gross_pay = regular_pay + overtime_pay;

                        float penalty_per_absence = 8.0 * employees[i].hourly_salary;
                        float total_penalties = unexcused_absences * penalty_per_absence;

                        float net_pay = gross_pay - total_penalties;

                        if (net_pay < 0)
                        {
                            net_pay = 0;
                        }

                        fprintf(payroll_file, "%-5.3d %-25s $%-11.2f %-6d %-10.1f $%-11.2f $%-11.2f $%-11.2f $%-11.2f\n",
                                employees[i].emp_id,
                                employees[i].name,
                                employees[i].hourly_salary,
                                present_days,
                                regular_hours,
                                overtime_pay,
                                total_penalties,
                                gross_pay,
                                net_pay);

                        total_payroll += net_pay;
                    }
                }
            }

            fprintf(payroll_file, "------------------------------------------------------------------------------------------------------------\n");
            fprintf(payroll_file, "TOTAL PAYROLL FOR %s: $%.2f\n", months[m], total_payroll);
        }

        fprintf(payroll_file, "\n\nRaw Data (for system loading):\n");
        fprintf(payroll_file, "%d\n", month_count);
        for (int m = 0; m < month_count; m++)
        {
            for (int i = 0; i < employee_count; i++)
            {
                if (employees[i].is_active)
                {
                    float total_hours = 0;
                    int present_days = 0;
                    int unexcused_absences = 0;

                    for (int j = 0; j < attendance_count; j++)
                    {
                        if (attendance_records[j].emp_id == employees[i].emp_id)
                        {
                            char record_month[8];
                            strcpy(record_month, &attendance_records[j].date[3]);

                            if (strcmp(record_month, months[m]) == 0)
                            {
                                if (attendance_records[j].present)
                                {
                                    present_days++;
                                    total_hours += attendance_records[j].hours_worked;
                                }
                                else if (!attendance_records[j].excused)
                                {
                                    unexcused_absences++;
                                }
                            }
                        }
                    }

                    if (present_days > 0 || unexcused_absences > 0)
                    {
                        float standard_hours = present_days * 8.0;
                        float regular_hours = (total_hours <= standard_hours) ? total_hours : standard_hours;
                        float overtime_hours = (total_hours > standard_hours) ? (total_hours - standard_hours) : 0;

                        float regular_pay = regular_hours * employees[i].hourly_salary;
                        float overtime_pay = overtime_hours * employees[i].hourly_salary * 1.5;
                        float gross_pay = regular_pay + overtime_pay;

                        float penalty_per_absence = 8.0 * employees[i].hourly_salary;
                        float total_penalties = unexcused_absences * penalty_per_absence;

                        float net_pay = gross_pay - total_penalties;
                        if (net_pay < 0)
                            net_pay = 0;

                        fprintf(payroll_file, "%s|%03d|%d|%.2f|%.2f|%.2f|%.2f|%.2f\n",
                                months[m],
                                employees[i].emp_id,
                                present_days,
                                regular_hours,
                                overtime_hours,
                                gross_pay,
                                total_penalties,
                                net_pay);
                    }
                }
            }
        }
        fclose(payroll_file);
    }
}

void load_from_file()
{
    FILE *emp_file = fopen("employees.txt", "r");
    if (emp_file)
    {
        char line[256];
        int found_raw_data = 0;

        while (fgets(line, sizeof(line), emp_file))
        {
            if (strstr(line, "Raw Data (for system loading):"))
            {
                found_raw_data = 1;
                break;
            }
        }

        if (found_raw_data)
        {
            fscanf(emp_file, "%d", &employee_count);
            for (int i = 0; i < employee_count; i++)
            {
                fscanf(emp_file, "%d|%49[^|]|%49[^|]|%f|%d",
                       &employees[i].emp_id,
                       employees[i].name,
                       employees[i].position,
                       &employees[i].hourly_salary,
                       &employees[i].is_active);
            }
        }
        fclose(emp_file);
    }

    FILE *att_file = fopen("attendance.txt", "r");
    if (att_file)
    {
        char line[256];
        int found_raw_data = 0;

        while (fgets(line, sizeof(line), att_file))
        {
            if (strstr(line, "Raw Data (for system loading):"))
            {
                found_raw_data = 1;
                break;
            }
        }

        if (found_raw_data)
        {
            fscanf(att_file, "%d", &attendance_count);
            for (int i = 0; i < attendance_count; i++)
            {
                fscanf(att_file, "%d|%14[^|]|%f|%d|%d",
                       &attendance_records[i].emp_id,
                       attendance_records[i].date,
                       &attendance_records[i].hours_worked,
                       &attendance_records[i].present,
                       &attendance_records[i].excused);
            }
        }
        fclose(att_file);
    }
}

int get_next_id()
{
    int max_id = 0;
    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].emp_id > max_id)
        {
            max_id = employees[i].emp_id;
        }
    }
    return max_id + 1;
}

void mark_daily_attendance()
{
    printf("\n============================================\n");
    printf("           MARK DAILY ATTENDANCE           \n");
    printf("============================================\n");

    if (employee_count == 0)
    {
        printf("No employees found! Please add employees first.\n");
        return;
    }

    char date[15];
    printf("Enter Date (DD/MM/YYYY): ");
    scanf("%s", date);

    int date_exists = 0;
    for (int i = 0; i < attendance_count; i++)
    {
        if (strcmp(attendance_records[i].date, date) == 0)
        {
            date_exists = 1;
            break;
        }
    }

    if (date_exists)
    {
        char confirm;
        printf("Attendance records for %s already exist. Update? (y/n): ", date);
        scanf(" %c", &confirm);
        if (confirm != 'y' && confirm != 'Y')
        {
            printf("Operation cancelled.\n");
            return;
        }
    }

    printf("\n============================================\n");
    printf("         ATTENDANCE FOR %s            \n", date);
    printf("============================================\n");
    printf("Active Employees:\n");
    printf("%-5s %-25s %-20s\n", "ID", "Name", "Position");
    printf("-----------------------------------------------------\n");

    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].is_active)
        {
            printf("%-5.3d %-25s %-20s\n",
                   employees[i].emp_id,
                   employees[i].name,
                   employees[i].position);
        }
    }
    printf("-----------------------------------------------------\n");

    printf("\nInstructions:\n");
    printf("- Enter Employee ID followed by present status (1=Present, 0=Absent)\n");
    printf("- If present, you'll be asked for hours worked\n");
    printf("- Enter 0 as Employee ID to finish\n");
    printf("--------------------------------------------\n");

    int emp_id;
    int records_marked = 0;

    while (1)
    {
        printf("Enter Employee ID (0 to finish): ");
        scanf("%d", &emp_id);

        if (emp_id == 0)
        {
            break;
        }

        int found = 0;
        char emp_name[MAX_STRING];
        for (int i = 0; i < employee_count; i++)
        {
            if (employees[i].emp_id == emp_id && employees[i].is_active)
            {
                found = 1;
                strcpy(emp_name, employees[i].name);
                break;
            }
        }

        if (!found)
        {
            printf("Employee with ID %03d not found or inactive! Try again.\n", emp_id);
            continue;
        }

        int existing_record_index = -1;
        for (int i = 0; i < attendance_count; i++)
        {
            if (attendance_records[i].emp_id == emp_id &&
                strcmp(attendance_records[i].date, date) == 0)
            {
                existing_record_index = i;
                break;
            }
        }

        printf("Employee: %s (ID: %03d)\n", emp_name, emp_id);

        int present;
        printf("Is present? (1=Yes, 0=No): ");
        scanf("%d", &present);

        if (present != 0 && present != 1)
        {
            printf("Invalid input! Please enter 1 for present or 0 for absent.\n");
            continue;
        }

        float hours_worked = 0;
        int excused = 1;

        if (present)
        {
            printf("Enter Hours Worked (0-24): ");
            scanf("%f", &hours_worked);

            if (hours_worked < 0 || hours_worked > 24)
            {
                printf("Invalid hours! Setting to 0.\n");
                hours_worked = 0;
            }
        }
        else
        {
            printf("Is this absence excused? (1=Yes/Excused, 0=No/Unexcused): ");
            scanf("%d", &excused);

            if (excused != 0 && excused != 1)
            {
                printf("Invalid input! Setting to unexcused (penalty will apply).\n");
                excused = 0;
            }

            printf("Absence marked as: %s\n", excused ? "Excused (No penalty)" : "Unexcused (Penalty will apply)");
        }

        if (existing_record_index != -1)
        {
            attendance_records[existing_record_index].present = present;
            attendance_records[existing_record_index].hours_worked = hours_worked;
            attendance_records[existing_record_index].excused = excused;
            printf("Updated: %s - %s, Hours: %.2f",
                   emp_name,
                   present ? "Present" : "Absent",
                   hours_worked);
            if (!present)
            {
                printf(" (%s)", excused ? "Excused" : "Unexcused");
            }
            printf("\n");
        }
        else
        {
            if (attendance_count >= MAX_ATTENDANCE)
            {
                printf("Attendance record limit reached!\n");
                break;
            }

            Attendance new_record;
            new_record.emp_id = emp_id;
            strcpy(new_record.date, date);
            new_record.present = present;
            new_record.hours_worked = hours_worked;
            new_record.excused = excused;

            attendance_records[attendance_count] = new_record;
            attendance_count++;

            printf("Recorded: %s - %s, Hours: %.2f",
                   emp_name,
                   present ? "Present" : "Absent",
                   hours_worked);
            if (!present)
            {
                printf(" (%s)", excused ? "Excused" : "Unexcused");
            }
            printf("\n");
        }

        records_marked++;
        printf("--------------------------------------------\n");
    }

    if (records_marked > 0)
    {
        printf("\nAttendance marking completed!\n");
        printf("Date: %s\n", date);
        printf("Records processed: %d\n", records_marked);

        printf("\n============================================\n");
        printf("        SUMMARY FOR %s             \n", date);
        printf("============================================\n");
        printf("%-5s %-25s %-10s %-8s %-15s\n", "ID", "Name", "Status", "Hours", "Absence Type");
        printf("----------------------------------------------------------------------\n");

        for (int i = 0; i < attendance_count; i++)
        {
            if (strcmp(attendance_records[i].date, date) == 0)
            {
                char name[MAX_STRING] = "Unknown";
                for (int j = 0; j < employee_count; j++)
                {
                    if (employees[j].emp_id == attendance_records[i].emp_id)
                    {
                        strcpy(name, employees[j].name);
                        break;
                    }
                }

                printf("%-5.3d %-25s %-10s %-8.2f",
                       attendance_records[i].emp_id,
                       name,
                       attendance_records[i].present ? "Present" : "Absent",
                       attendance_records[i].hours_worked);

                if (!attendance_records[i].present)
                {
                    printf(" %-15s", attendance_records[i].excused ? "Excused" : "Unexcused");
                }
                else
                {
                    printf(" %-15s", "N/A");
                }
                printf("\n");
            }
        }
        printf("----------------------------------------------------------------------\n");
    }
    else
    {
        printf("No attendance records were marked.\n");
    }
}

void calculate_payroll()
{
    printf("\n============================================\n");
    printf("           PAYROLL CALCULATION             \n");
    printf("============================================\n");
    if (employee_count == 0)
    {
        printf("No employees found!\n");
        return;
    }

    char month[20];
    printf("Enter month for payroll (MM/YYYY): ");
    scanf("%s", month);

    printf("\n============================================\n");
    printf("           PAYROLL FOR %s             \n", month);
    printf("============================================\n");
    printf("%-5s %-25s %-12s %-6s %-10s %-12s %-12s %-12s %-12s\n",
           "ID", "Name", "Hourly-Rate", "Days", "Reg-Hours", "Overtime-Pay", "Penalties", "Gross-Pay", "Net-Pay");
    printf("------------------------------------------------------------------------------------------------------------\n");

    float total_payroll = 0;

    for (int i = 0; i < employee_count; i++)
    {
        if (employees[i].is_active)
        {
            float total_hours = 0;
            int present_days = 0;
            int unexcused_absences = 0;

            for (int j = 0; j < attendance_count; j++)
            {
                if (attendance_records[j].emp_id == employees[i].emp_id)
                {
                    char record_month[8];
                    strcpy(record_month, &attendance_records[j].date[3]);

                    if (strcmp(record_month, month) == 0)
                    {
                        if (attendance_records[j].present)
                        {
                            present_days++;
                            total_hours += attendance_records[j].hours_worked;
                        }
                        else if (!attendance_records[j].excused)
                        {
                            unexcused_absences++;
                        }
                    }
                }
            }

            float standard_hours = present_days * 8.0;
            float regular_hours = (total_hours <= standard_hours) ? total_hours : standard_hours;
            float overtime_hours = (total_hours > standard_hours) ? (total_hours - standard_hours) : 0;

            float regular_pay = regular_hours * employees[i].hourly_salary;
            float overtime_pay = overtime_hours * employees[i].hourly_salary * 1.5;
            float gross_pay = regular_pay + overtime_pay;

            float penalty_per_absence = 8.0 * employees[i].hourly_salary;
            float total_penalties = unexcused_absences * penalty_per_absence;

            float net_pay = gross_pay - total_penalties;

            if (net_pay < 0)
            {
                net_pay = 0;
            }

            printf("%-5.3d %-25s $%-11.2f %-6d %-10.1f $%-11.2f $%-11.2f $%-11.2f $%-11.2f\n",
                   employees[i].emp_id,
                   employees[i].name,
                   employees[i].hourly_salary,
                   present_days,
                   regular_hours,
                   overtime_pay,
                   total_penalties,
                   gross_pay,
                   net_pay);

            total_payroll += net_pay;
        }
    }

    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("TOTAL PAYROLL FOR %s: $%.2f\n", month, total_payroll);
    printf("\nPayroll Calculation Rules:\n");
    printf("- Regular pay: Hourly rate x hours worked (up to 8 hours/day)\n");
    printf("- Overtime: 1.5x hourly rate for hours > 8 per day\n");
    printf("- Penalty: 1 day's pay (8 hours) deducted for each unexcused absence\n");
    printf("- Excused absences: No penalty applied\n");
    printf("- Net pay cannot go below $0.00\n");
}

void clear_screen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause()
{
    printf("\nPress Enter to continue... ");
    getchar();
    getchar();
}

// AUTHENTICATION FUNCTIONS

int authenticate_user()
{
    char username[MAX_STRING];
    char password[MAX_PASSWORD];

    printf("\n============================================\n");
    printf("              LOGIN SYSTEM                 \n");
    printf("============================================\n");

    if (user_count == 0)
    {
        printf("ERROR: No user accounts found in users.txt!\n");
        printf("Please ensure users.txt exists and contains valid user data.\n");
        pause();
    }

    printf("Users loaded: %d accounts found\n", user_count);

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    for (int i = 0; i < user_count; i++)
    {
        if (users[i].is_active &&
            strcmp(users[i].username, username) == 0 &&
            strcmp(users[i].password, password) == 0)
        {
            current_user = &users[i];
            is_logged_in = 1;
            return 1;
        }
    }
    return 0;
}

void login_menu()
{
    clear_screen();

    printf("\n");
    printf("    ███████╗███╗   ███╗██████╗ ██╗      ██████╗ ██╗   ██╗███████╗███████╗    ███╗   ███╗ █████╗ ███╗   ██╗ █████╗  ██████╗ ███████╗██████╗ \n");
    printf("    ██╔════╝████╗ ████║██╔══██╗██║     ██╔═══██╗╚██╗ ██╔╝██╔════╝██╔════╝    ████╗ ████║██╔══██╗████╗  ██║██╔══██╗██╔════╝ ██╔════╝██╔══██╗\n");
    printf("    █████╗  ██╔████╔██║██████╔╝██║     ██║   ██║ ╚████╔╝ █████╗  █████╗      ██╔████╔██║███████║██╔██╗ ██║███████║██║  ███╗█████╗  ██████╔╝\n");
    printf("    ██╔══╝  ██║╚██╔╝██║██╔═══╝ ██║     ██║   ██║  ╚██╔╝  ██╔══╝  ██╔══╝      ██║╚██╔╝██║██╔══██║██║╚██╗██║██╔══██║██║   ██║██╔══╝  ██╔══██╗\n");
    printf("    ███████╗██║ ╚═╝ ██║██║     ███████╗╚██████╔╝   ██║   ███████╗███████╗    ██║ ╚═╝ ██║██║  ██║██║ ╚████║██║  ██║╚██████╔╝███████╗██║  ██║\n");
    printf("    ╚══════╝╚═╝     ╚═╝╚═╝     ╚══════╝ ╚═════╝    ╚═╝   ╚══════╝╚══════╝    ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
    printf("\n");

    int attempts = 0;
    const int max_attempts = 3;

    while (attempts < max_attempts && !is_logged_in)
    {
        if (attempts > 0)
        {
            printf("\nLogin failed! Attempts remaining: %d\n", max_attempts - attempts);
        }

        if (authenticate_user())
        {
            printf("\nLogin successful!\n");
            printf("Welcome, %s (%s)!\n", current_user->username, current_user->role);
            pause();
            return;
        }
        else
        {
            attempts++;
            if (attempts < max_attempts)
            {
                printf("\nInvalid username or password!\n");
                pause();
                clear_screen();

                printf("╔═══════════════════════════════════════════════════════════════════════════╗\n");
                printf("║                              LOGIN REQUIRED                               ║\n");
                printf("╚═══════════════════════════════════════════════════════════════════════════╝\n");
                printf("\n");
            }
        }
    }

    if (attempts >= max_attempts)
    {
        printf("\nMaximum login attempts exceeded. Exiting system...\n");
        pause();
    }
}

void change_password()
{
    char current_password[MAX_PASSWORD];
    char new_password[MAX_PASSWORD];
    char confirm_password[MAX_PASSWORD];

    printf("\n============================================\n");
    printf("            CHANGE PASSWORD                 \n");
    printf("============================================\n");

    printf("Current Password: ");
    scanf("%s", current_password);

    if (strcmp(current_user->password, current_password) != 0)
    {
        printf("\nCurrent password is incorrect!\n");
        pause();
    }

    printf("New Password: ");
    scanf("%s", new_password);

    printf("Confirm New Password: ");
    scanf("%s", confirm_password);

    if (strcmp(new_password, confirm_password) != 0)
    {
        printf("\nNew passwords do not match!\n");
        pause();
    }

    strcpy(current_user->password, new_password);
    printf("\nPassword changed successfully!\n");
    save_users_to_file();
    pause();
}

void logout()
{
    printf("\n============================================\n");
    printf("                 LOGOUT                     \n");
    printf("============================================\n");

    printf("Goodbye, %s!\n", current_user->username);
    current_user = NULL;
    is_logged_in = 0;

    printf("You have been logged out successfully.\n");
    pause();
}

int has_permission(const char *required_role)
{
    if (!current_user || !is_logged_in)
        return 0;

    return (strcmp(current_user->role, required_role) == 0);
}

void save_users_to_file()
{
    FILE *user_file = fopen("users.txt", "w");
    if (user_file)
    {
        fprintf(user_file, "%d\n", user_count);
        for (int i = 0; i < user_count; i++)
        {
            fprintf(user_file, "%s %s %s %d\n",
                    users[i].username,
                    users[i].password,
                    users[i].role,
                    users[i].is_active);
        }
        fclose(user_file);
    }
    else
    {
        printf("Error: Could not save to users.txt\n");
    }
}

void load_users_from_file()
{
    printf("DEBUG: Function load_users_from_file() called\n");

    FILE *user_file = fopen("users.txt", "r");
    if (!user_file)
    {
        printf("Error: Cannot open users.txt file\n");
        user_count = 0;
        return;
    }

    printf("DEBUG: File opened successfully\n");

    if (fscanf(user_file, "%d", &user_count) != 1)
    {
        printf("Error: Invalid user count in users.txt\n");
        user_count = 0;

        return;
    }

    printf("DEBUG: User count read: %d\n", user_count);
    int count = 0;
    while (fscanf(user_file, "%s %s %s %d",
                  users[count].username,
                  users[count].password,
                  users[count].role,
                  &users[count].is_active) == 4)
    {

        count++;
        if (count >= user_count)
            break;
    }

    fclose(user_file);

    printf("DEBUG: Users loaded successfully\n");
}
