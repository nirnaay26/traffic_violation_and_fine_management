#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_VEHICLES 1000
#define MAX_LOCATIONS 50
#define MAX_TYPES 4

#define REG_NO_SIZE 30
#define NAME_SIZE 100
#define CONTACT_SIZE 11
#define DATETIME_SIZE 20
#define MODE_SIZE 30
#define DATE_SIZE 12
 
// defining all structures require for project
typedef enum
{   ILLEGAL_PARKING = 1,
    OVER_SPEEDING,
    RED_LIGHT_JUMP,
    HELMET_SEATBELT
} ViolationType;

typedef struct Location
{    int locationID;
    char name[NAME_SIZE];
    int violationCount;
    double totalFineRevenue;
    struct Location *next;
} Location;

typedef struct Vehicle
{   char regNo[REG_NO_SIZE];
    char ownerName[NAME_SIZE];
    char contact[CONTACT_SIZE];
    char address[NAME_SIZE];
    double totalFineAccrued;
    double totalFinePaid;
    double pendingAmount;
    int violationCount;
    struct Vehicle *next;
} Vehicle;

typedef struct Violation
{   int violationID;
    Vehicle *vehicle;
    ViolationType type;
    Location *location;
    char dateTime[DATETIME_SIZE];
    double fineAmount;
    int paid;
    struct Violation *next;

} Violation;

typedef struct Payment
{   int transactionID;
    Violation *violation;
    double amountPaid;
    char mode[MODE_SIZE];
    char date[DATE_SIZE];
    struct Payment *next;

} Payment;

// declare all global pointers
Vehicle *vehicleHead = NULL;
Violation *violationHead = NULL;
Location *locationHead = NULL;
Payment *paymentTop = NULL;

int nextViolationID = 1;
int nextTransactionID = 1;

const char* typeName(ViolationType type){
    switch (type){
         case 1:
            return "illegal parking";
        case 2:
            return "over-speeding";
        case 3:
            return "red-light jump";
        case 4:
            return "helmet/seatbelt infraction";
        default:
            return "unknown";
    }
}

double baseFine(ViolationType type){
     switch (type){
         case 1:
            return 500.0;
        case 2:
            return 1000.0;
        case 3:
            return 1500.0;
        case 4:
            return 500.0;
        default:
            return 0.0;
    }
}

Vehicle *findVehicle(const char *regNo){
    Vehicle *p = vehicleHead;
    while (p != NULL){
        if (strcmp(p->regNo, regNo) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

Location *findLocation(int id){
    Location *p = locationHead;
    while (p != NULL){
        if (p->locationID == id){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

Violation *findViolation(int id){
    Violation *p = violationHead;
    while (p != NULL){
        if (p->violationID == id){
            return p;
        }
        p = p->next;
    }
    return NULL;
}

int countUnpaid(Vehicle *v){
    int count = 0;
    Violation *p = violationHead;
    while (p != NULL){
         if (p->vehicle == v && p->paid == 0){
            count++;
        }
        p = p->next;
    }
    return count;
}

void printVehicle(Vehicle *v){
    printf("registration : %s\n", v->regNo);
    printf("owner : %s\n", v->ownerName);
    printf("contact : %s\n", v->contact);
    printf("address : %s\n", v->address);
    printf("violations : %d\n", v->violationCount);
    printf("accrued fine : Rs. %f\n", v->totalFineAccrued);
    printf("paid fine  : Rs. %f\n", v->totalFinePaid);
    printf("pending : Rs. %f\n", v->pendingAmount);
}

void printViolation(Violation *v){
    printf("ID: %d | vehicle: %s | type: %s | location: %s | date: %s | fine: Rs. %f | %s\n",
         v->violationID, v->vehicle->regNo, typeName(v->type), v->location->name, v->dateTime, v->fineAmount, v->paid ? "PAID" : "UNPAID");
}

Vehicle *getMiddleVehicle(Vehicle *head){
    if (head == NULL || head->next == NULL){
        return head;
    }
    Vehicle *slow = head;
    Vehicle *fast = head->next;
    while (fast != NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

Vehicle *mergeVehicles(Vehicle *a, Vehicle *b){
    Vehicle *result = NULL;
    Vehicle *tail = NULL;
    if (a == NULL) return b;
    if (b == NULL) return a;
    if (strcmp(a->regNo, b->regNo) <= 0){
        result = a;
        a = a->next;
    }
    else{
        result = b;
        b = b->next;
    }
    tail = result;
    while (a != NULL && b != NULL){
        if (strcmp(a->regNo, b->regNo) <= 0){
            tail->next = a;
            a = a->next;
        }
        else{
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL){
        tail->next = a;
    }
    else{
        tail->next = b;
    }
    return result;
}

Vehicle *mergeSortVehicles(Vehicle *head){
    if (head == NULL || head->next == NULL){
        return head;
    }
    Vehicle *middle= getMiddleVehicle(head);
    Vehicle *secondHalf = middle->next;
    middle->next = NULL;
    Vehicle *left = mergeSortVehicles(head);
    Vehicle *right = mergeSortVehicles(secondHalf);
    return mergeVehicles(left, right);
}

// methods performing in vehicle traffic system

void registerVehicle(){
    Vehicle *n;
    n = (Vehicle *)malloc(sizeof(Vehicle));

    if (n == NULL){
        printf("memory allocation failed\n");
        return;
    }
    printf("Enter registration number:\n ");
    scanf("%s", n->regNo);

    if (findVehicle(n->regNo) != NULL){
        printf("vehicle already exists.\n");
        free(n);
        return;
    }
    printf("enter owner name: ");
    scanf(" %s", n->ownerName);

    printf("enter contact: ");
    scanf("%s", n->contact);

    printf("enter address: ");
    scanf(" %s", n->address);

    n->totalFineAccrued = 0;
    n->totalFinePaid = 0;
    n->pendingAmount = 0;
    n->violationCount = 0;
    n->next = vehicleHead;
    vehicleHead = n;
    vehicleHead = mergeSortVehicles(vehicleHead);
    printf("vehicle registrered succcessfully\n");
}

void addLocation(){
    Location *n;
    n = (Location *)malloc(sizeof(Location));
    if (n == NULL){
        printf("memory allocation failed.\n");
        return;
    }

    printf("enter location ID:\n");
    scanf("%d", &n->locationID);
    if (findLocation(n->locationID) != NULL){
        printf("Location already exists.\n");
        free(n);
        return;
    }
    printf("enter location name: ");
    scanf(" %s", n->name);

    n->violationCount = 0;
    n->totalFineRevenue = 0;
    n->next = locationHead;
    locationHead = n;
    printf("Location added successfully.\n");
}

void addViolation(){
    char regNo[30];
    int locationID;
    int choice;
    printf("enter vehicle registration number:\n");
    scanf(" %s", regNo);

    Vehicle *vehicle = findVehicle(regNo);
    if (vehicle == NULL){
        printf("Vehicle not found.\n");
        return;
    }
    printf("Enter location ID: ");
    scanf("%d", &locationID);

    Location *location = findLocation(locationID);
    if (location == NULL){
        printf("Location not found.\n");
        return;
    }
    printf("1 illegal parking\n");
    printf("2 over-speeding\n");
    printf("3 red-light jump\n");
    printf("4. helmet/seatbelt infraction\n");
    printf("enter violation type: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > 4){
        printf("Invalid violation type.\n");
        return;
    }
    Violation *n;
    n = (Violation *)malloc(sizeof(Violation));
    if (n == NULL){
        printf("Memory allocation failed.\n");
        return;
    }
    n->violationID = nextViolationID++;
    n->vehicle = vehicle;
    n->type = (ViolationType)choice;
    n->location = location;

    printf("enter date and time (DD-MM-YYYY/HH:MM): ");
    scanf(" %s", n->dateTime);

    n->fineAmount = baseFine(n->type);
    if (vehicle->violationCount >= 2){
        n->fineAmount += 200;
    }
    n->paid = 0;
    n->next = violationHead;
    violationHead = n;
    vehicle->violationCount++;
    vehicle->totalFineAccrued += n->fineAmount;
    vehicle->pendingAmount += n->fineAmount;
    location->violationCount++;
    location->totalFineRevenue += n->fineAmount;

    printf("violation added successfully\n");
    printf("violation ID = %d\n",n->violationID);
    printf("\nfine = Rs %f\n",n->fineAmount);
}

void makePayment(int voilationID){
    float amount;
    char mode[MODE_SIZE];
    char date[DATETIME_SIZE];

    Violation *v = findViolation(voilationID);
    if (v == NULL){
        printf("violation not found.\n");
        return;
    }
    if (v->paid){
        printf("violation already paid.\n");
        return;
    }
    printf("fine = Rs %f\n", v->fineAmount);
    printf("enter amount paid: ");
    scanf("%f", &amount);

    if (amount != v->fineAmount){
        printf("please enter the complete fine amount.\n");
        return;
    }
    printf("enter payment mode: ");
    scanf("%s", mode);
    printf("enter payment date: ");
    scanf("%s", date);

    Payment *p;
    p = (Payment *)malloc(sizeof(Payment));

    if (p == NULL){
        printf("memory allocation failed.\n");
        return;
    }
    p->transactionID = nextTransactionID++;
    p->violation = v;
    p->amountPaid = amount;
    strcpy(p->mode, mode);
    strcpy(p->date, date);

    p->next = paymentTop;
    paymentTop = p;
    v->paid = 1;
    v->vehicle->totalFinePaid += amount;
    v->vehicle->pendingAmount -= amount;
    printf("payment successful\n");
    printf("transaction ID = %d\n", p->transactionID);
}

void ownerVoilations(char owner[]){
    int vehicleFound=0;

   Vehicle *v = vehicleHead;
    while (v != NULL)
    {   if (strcmp(v->ownerName, owner) == 0){
            vehicleFound = 1;
            printf("vehicle: %s\n",v->regNo);

            Violation *x = violationHead;
            int found = 0;
            while (x != NULL)
            {   if (x->vehicle == v){
                    printViolation(x);
                    found = 1;
                }
                x = x->next;
            }
            if (!found){
                printf("No violations.\n");
            }
        }
        v = v->next;
    }
    if (!vehicleFound){
        printf("No vehicle found for this owner.\n");
    }
}

void locationViolations(int id){

    Location *loc = findLocation(id);
    if (loc == NULL){
        printf("Location not found.\n");
        return;
    }
    printf("\nLocation: %s\n",loc->name);

    Violation *v = violationHead;
    int found = 0;

    while (v != NULL)
    {   if (v->location == loc){
            printViolation(v);
            found = 1;
        }
        v = v->next;
    }
    if (!found){
         printf("No violations at this location.\n");
    }
}

Vehicle *mergeByUnpaidViolation(Vehicle *a, Vehicle *b){
    Vehicle *result = NULL;
    Vehicle *tail = NULL;
    if (a == NULL){
        return b;
    }

    if (b == NULL){
        return a;
    }

    if (countUnpaid(a) >= countUnpaid(b)){
        result = a;
        a = a->next;
    }
    else
    {   result = b;
        b = b->next;
    }
    tail = result;

    while (a != NULL && b != NULL){
        if (countUnpaid(a) >= countUnpaid(b)){
            tail->next = a;
            a = a->next;
        }
        else
        {   tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL){
        tail->next = a;
    }
    else{
        tail->next = b;
    }
    return result;
}

Vehicle *mergeSortVehiclesByUnpaidVoilation(Vehicle *head){
    if (head == NULL || head->next == NULL){
        return head;
    }
    Vehicle *middle = getMiddleVehicle(head);
    Vehicle *secondHalf = middle->next;

    middle->next = NULL;

    Vehicle *left = mergeSortVehiclesByUnpaidVoilation(head);
    Vehicle *right = mergeSortVehiclesByUnpaidVoilation(secondHalf);

    return mergeByUnpaidViolation(left, right);
}

void sortVehiclesByUnpaidViolations(){
    vehicleHead = mergeSortVehiclesByUnpaidVoilation(vehicleHead);
    printf("\nVehicles sorted by unpaid violations:\n");

    Vehicle *p = vehicleHead;
    while (p != NULL){
        printf("%s | Unpaid Violations = %d | Pending = Rs. %f\n",p->regNo, countUnpaid(p), p->pendingAmount);
        p = p->next;
    }
    vehicleHead = mergeSortVehicles(vehicleHead);
}

Location *getMiddleLocation(Location *head){
    if (head == NULL || head->next == NULL){
        return head;
    }
    Location *slow = head;
    Location *fast = head->next;
    while (fast != NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

Location *mergeLocationsByCount(Location *a, Location *b){
    Location *result = NULL;
    Location *tail = NULL;
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    if (a->violationCount >= b->violationCount){
        result = a;
        a = a->next;
    }
    else
    {   result = b;
        b = b->next;
    }
    tail = result;
    while (a != NULL && b != NULL){
        if (a->violationCount >= b->violationCount){
            tail->next = a;
            a = a->next;
        }
        else
        {   tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL)
        tail->next = a;
    else
        tail->next = b;

    return result;
}

Location *mergeSortLocationsByCount(Location *head){
    if (head == NULL || head->next == NULL)
        return head;
    Location *middle = getMiddleLocation(head);
    Location *secondHalf = middle->next;

    middle->next = NULL;
    Location *left = mergeSortLocationsByCount(head);
    Location *right = mergeSortLocationsByCount(secondHalf);
    return mergeLocationsByCount(left, right);
}

void sortLocationsByCount()
{
    locationHead = mergeSortLocationsByCount(locationHead);

    printf("locations by number of violations:\n");
    Location *p = locationHead;
    while (p != NULL){
        printf("%d | %s | Violations = %d\n", p->locationID, p->name, p->violationCount);
        p = p->next;
    }
}

Location *mergeLocationsByRevenue(Location *a, Location *b){
    Location *result = NULL;
    Location *tail = NULL;
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;

    if (a->totalFineRevenue >= b->totalFineRevenue){
        result = a;
        a = a->next;
    }
    else
    {   result = b;
        b = b->next;
    }
    tail = result;
    while (a != NULL && b != NULL){
        if (a->totalFineRevenue >= b->totalFineRevenue){
            tail->next = a;
            a = a->next;
        }
        else
        {   tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL)
        tail->next = a;
    else
        tail->next = b;

    return result;
}

Location *mergeSortLocationsByRevenue(Location *head)
{
    if (head == NULL || head->next == NULL)
        return head;

    Location *middle = getMiddleLocation(head);
    Location *secondHalf = middle->next;

    middle->next = NULL;

    Location *left = mergeSortLocationsByRevenue(head);
    Location *right = mergeSortLocationsByRevenue(secondHalf);

    return mergeLocationsByRevenue(left, right);
}

void sortLocationsByRevenue(){
    locationHead = mergeSortLocationsByRevenue(locationHead);
    printf("locations by fine revenue:\n");
    Location *p = locationHead;
    while (p != NULL){
        printf("%d | %s | Revenue = Rs. %f\n", p->locationID, p->name, p->totalFineRevenue);
        p = p->next;
    }
}

Violation *mergeViolationsByFrequency(Violation *a, Violation *b,int count[]){
    Violation *result = NULL;
    Violation *tail = NULL;
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;
    if (count[a->type - 1] >= count[b->type - 1]){
        result = a;
        a = a->next;
    }
    else
    {   result = b;
        b = b->next;
    }
    tail = result;
    while (a != NULL && b != NULL){
        if (count[a->type - 1] >= count[b->type - 1])
        {
            tail->next = a;
            a = a->next;
        }
        else
        {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL)
        tail->next = a;
    else
        tail->next = b;
    return result;
}

Violation *getMiddleViolation(Violation *head){
    if (head == NULL || head->next == NULL)
        return head;

    Violation *slow = head;
    Violation *fast = head->next;

    while (fast != NULL && fast->next != NULL){
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

Violation *mergeSortViolationsByFrequency(Violation *head, int count[]){
    if (head == NULL || head->next == NULL)
        return head;
    Violation *middle = getMiddleViolation(head);
    Violation *secondHalf = middle->next;

    middle->next = NULL;

    Violation *left = mergeSortViolationsByFrequency(head, count);
    Violation *right = mergeSortViolationsByFrequency(secondHalf, count);

    return mergeViolationsByFrequency(left, right, count);
}

void violationTypeFrequency(){
    int count[MAX_TYPES] = {0};
    Violation *v = violationHead;
    while (v != NULL){
        if (v->type >= 1 && v->type <= MAX_TYPES)
            count[v->type-1]++;

        v = v->next;
    }

    violationHead = mergeSortViolationsByFrequency(violationHead, count);
    printf("violations sorted by type frequency:\n");
    v = violationHead;

    if (v == NULL){
        printf("No violations.\n");
        return;
    }

    while (v != NULL){
        printf("Violation ID = %d | Type = %s | Frequency = %d\n", v->violationID, typeName(v->type), count[v->type - 1]);
        v = v->next;
    }
}

void repeatOffenderFractions(){
    int total[MAX_TYPES] = {0};
    int repeat[MAX_TYPES] = {0};

    Violation *v = violationHead;
    while (v != NULL){
        total[v->type - 1]++;
        if (v->vehicle->violationCount > 2){
            repeat[v->type-1]++;
        }
        v = v->next;
    }


    typedef struct{
        ViolationType type;
        int total;
        int repeat;
        double percentage;
    } RepeatStat;

    RepeatStat a[MAX_TYPES];

    for (int i = 0; i < MAX_TYPES; i++){
        a[i].type =(ViolationType)(i + 1);
        a[i].total = total[i];
        a[i].repeat = repeat[i];

        if (total[i] == 0){
            a[i].percentage = 0;
        }
        else{
            a[i].percentage =
                100.0 * repeat[i] / total[i];
        }
    }

    for (int i = 0; i < MAX_TYPES - 1; i++){
        for (int j = 0;j < MAX_TYPES - i - 1;j++){
            if (a[j].percentage < a[j + 1].percentage){
                RepeatStat temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
        }
    }

    printf("Repeat offender percentage:\n");
    for (int i = 0; i < MAX_TYPES; i++){
        printf("%s | Total = %d | Repeat = %d | %f\n",typeName(a[i].type),a[i].total,a[i].repeat, a[i].percentage);
    }
}

void vehiclesAtLeastThreshold(int threshold){

    Vehicle *v = vehicleHead;
    int found = 0;
    while (v != NULL){
        if (v->violationCount >= threshold){
            printf("%s | Owner: %s | Violations: %d | Pending: Rs. %f\n",v->regNo,v->ownerName,v->violationCount,v->pendingAmount);
            found = 1;
        }
        v = v->next;
    }
    if (!found)
    {
        printf("No matching vehicles.\n");
    }
}

void vehiclePendingViolations(){
    char regNo[30];
    printf("\nEnter registration number: ");
    scanf(" %s", regNo);

    Vehicle *vehicle =findVehicle(regNo);
    if (vehicle == NULL){
        printf("Vehicle not found.\n");
        return;
    }

    printf("pending violations:\n");
    Violation *v = violationHead;
    int found = 0;
    while (v != NULL){
        if (v->vehicle == vehicle && v->paid == 0){
            printViolation(v);
            found = 1;
        }
        v = v->next;
    }
    if (!found){
        printf("No pending violations.\n");
    }
}

Vehicle *mergeVehiclesByPendingFine(Vehicle *a, Vehicle *b)
{
    Vehicle *result = NULL;
    Vehicle *tail = NULL;
    if (a == NULL)
        return b;
    if (b == NULL)
        return a;
    if (a->pendingAmount >= b->pendingAmount){
        result = a;
        a = a->next;
    }
    else
    {    result = b;
        b = b->next;
    }
    tail = result;
    while (a != NULL && b != NULL){
        if (a->pendingAmount >= b->pendingAmount){
            tail->next = a;
            a = a->next;
        }
        else
        {    tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }
    if (a != NULL)
        tail->next = a;
    else
        tail->next = b;

    return result;
}

Vehicle *mergeSortVehiclesByPendingFine(Vehicle *head){
    if (head == NULL || head->next == NULL)
        return head;

    Vehicle *middle = getMiddleVehicle(head);
    Vehicle *secondHalf = middle->next;

    middle->next = NULL;
    Vehicle *left = mergeSortVehiclesByPendingFine(head);
    Vehicle *right = mergeSortVehiclesByPendingFine(secondHalf);
    return mergeVehiclesByPendingFine(left, right);
}

void sortVehiclesByPendingFine(){
    vehicleHead = mergeSortVehiclesByPendingFine(vehicleHead);
    printf("\nVehicles by pending fine:\n");
    Vehicle *v = vehicleHead;
    while (v != NULL){
        printf("%s | Pending = Rs. %f\n", v->regNo, v->pendingAmount);
        v = v->next;
    }
}

void paymentHistory(){
    if (paymentTop == NULL){
        printf("No payments recorded.\n");
        return;
    }
    printf("payment History (latest first):\n");

    Payment *p = paymentTop;
    while (p != NULL){
        printf("Transaction ID: %d |Violation ID: %d | Vehicle: %s | Amount: Rs. %f | Mode: %s | Date: %s\n",
            p->transactionID,p->violation->violationID, p->violation->vehicle->regNo, p->amountPaid,p->mode, p->date);

         p = p->next;
    }
}

void displayAllVehicles(){
    Vehicle *v = vehicleHead;
    if (v == NULL){
        printf("No vehicles.\n");
        return;
    }
    while (v != NULL){
        printVehicle(v);
        v = v->next;
    }
}

void displayAllLocations(){
    Location *p = locationHead;
    if (p == NULL){
        printf("No locations.\n");
        return;
    }
    while (p != NULL){
        printf("ID: %d | %s | Violations: %d | Revenue: Rs. %f\n",
            p->locationID,p->name,p->violationCount,p->totalFineRevenue);

        p = p->next;
    }
}

void displayAllViolations(){
    Violation *v = violationHead;
    if (v == NULL){
        printf("No violations.\n");
        return;
    }
    while (v != NULL){
        printViolation(v);
        v = v->next;
    }
}

void saveVehicles(){
    FILE *fp = fopen("vehicle.txt", "w");
    Vehicle *v = vehicleHead;

    if (fp == NULL){
        printf("Error opening vehicle.txt\n");
        return;
    }
    while (v != NULL){
        fprintf(fp, "%s %s %s %s %f %f %f %d\n",v->regNo, v->ownerName, v->contact, v->address,v->totalFineAccrued,
            v->totalFinePaid,v->pendingAmount, v->violationCount);
        v = v->next;
    }
    fclose(fp);
}

void saveLocations(){
    FILE *fp = fopen("location.txt", "w");
    Location *l = locationHead;

    if (fp == NULL){
        printf("Error opening location.txt\n");
        return;
    }

    while (l != NULL){
        fprintf(fp, "%d %s %d %f\n",l->locationID, l->name, l->violationCount, l->totalFineRevenue);
        l = l->next;
    }
    fclose(fp);
}

void saveViolations(){
    FILE *fp = fopen("voilation.txt", "w");
    Violation *v = violationHead;
    if (fp == NULL){
        printf("Error opening voilation.txt\n");
        return;
    }
    while (v != NULL){
        fprintf(fp, "%d %s %d %d %s %f %d\n",v->violationID, v->vehicle->regNo,(int)v->type, v->location->locationID,
            v->dateTime, v->fineAmount, v->paid);
        v = v->next;
    }
    fclose(fp);
}

void savePayments(){
    FILE *fp = fopen("payment.txt", "w");
    Payment *p = paymentTop;

    if (fp == NULL){
        printf("Error opening payment.txt\n");
        return;
    }
    while (p != NULL){
        fprintf(fp, "%d %d %f %s %s\n",p->transactionID, p->violation->violationID, p->amountPaid, p->mode, p->date);
        p = p->next;
    }
    fclose(fp);
}

void saveAllData(){
    saveVehicles();
    saveLocations();
    saveViolations();
    savePayments();
}

void loadVehicles(){
    FILE *fp = fopen("vehicle.txt", "r");
    if(fp == NULL){
        printf("error");
        return;
    }
    Vehicle *v, *tail = NULL;
    char regNo[REG_NO_SIZE];
    char owner[NAME_SIZE];
    char contact[CONTACT_SIZE];
    char address[NAME_SIZE];
    double accrued, paid, pending;
    int count;

    while (fscanf(fp,"%s %s %s %s %lf %lf %lf %d",regNo, owner, contact, address,
                       &accrued, &paid, &pending, &count) == 8){

        v = (Vehicle *)malloc(sizeof(Vehicle));
        if(v == NULL){
            printf("Memory allocation failed\n");
            fclose(fp);
            return;
        }
        strcpy(v->regNo, regNo);
        strcpy(v->ownerName, owner);
        strcpy(v->contact, contact);
        strcpy(v->address, address);

        v->totalFineAccrued = accrued;
        v->totalFinePaid = paid;
        v->pendingAmount = pending;
        v->violationCount = count;
        v->next = NULL;

        if(vehicleHead == NULL)
            vehicleHead = v;
        else
            tail->next = v;

        tail = v;
    }
    fclose(fp);
    vehicleHead = mergeSortVehicles(vehicleHead);
}

void loadLocations(){
    FILE *fp = fopen("location.txt", "r");
    Location *l, *tail = NULL;
    int id, count;
    char name[NAME_SIZE];
    double revenue;

    while (fscanf(fp, "%d %s %d %lf", &id, name, &count, &revenue) == 4)
    {
        l = (Location *)malloc(sizeof(Location));
        l->locationID = id;
        strcpy(l->name, name);
        l->violationCount = count;
        l->totalFineRevenue = revenue;
        l->next = NULL;

        if (locationHead == NULL)
            locationHead = l;
        else
            tail->next = l;

        tail = l;
    }
    fclose(fp);
}

void loadPayments(){
    FILE *fp = fopen("payment.txt", "r");
    Payment *p, *tail = NULL;
    int transactionID, violationID;
    double amount;
    char mode[MODE_SIZE];
    char date[DATE_SIZE];

    if (fp == NULL){
        printf("Error opening payment.txt\n");
        return;
    }
    while (fscanf(fp, "%d %d %lf %s %s",&transactionID, &violationID, &amount, mode, date) == 5){
        Violation *violation = findViolation(violationID);

        if (violation == NULL)
            continue;
        p = (Payment *)malloc(sizeof(Payment));

        if (p == NULL){
            printf("Memory allocation failed\n");
            fclose(fp);
            return;
        }
        p->transactionID = transactionID;
        p->violation = violation;
        p->amountPaid = amount;
        strcpy(p->mode, mode);
        strcpy(p->date, date);
        p->next = NULL;

        if (paymentTop == NULL)
            paymentTop = p;
        else
            tail->next = p;
        tail = p;

        if (transactionID >= nextTransactionID)
            nextTransactionID = transactionID + 1;
    }
    fclose(fp);
}

void loadViolations(){
    FILE *fp = fopen("voilation.txt", "r");
    Violation *v, *tail = NULL;
    char regNo[REG_NO_SIZE];
    char dateTime[DATETIME_SIZE];
    int id, type, locationID, paid;
    double fine;

    if (fp == NULL){
        printf("Error opening voilation.txt\n");
        return;
    }
   while (fscanf(fp, "%d %29s %d %d %19s %lf %d", &id, regNo, &type, &locationID, dateTime, &fine, &paid) == 7){
    Vehicle *vehicle = findVehicle(regNo);
    Location *location = findLocation(locationID);
    if (vehicle == NULL || location == NULL) continue;
        v = (Violation *)malloc(sizeof(Violation));
        if (v == NULL){
            printf("Memory allocation failed\n");
            fclose(fp);
            return;
        }
        v->violationID = id;
        v->vehicle = vehicle;
        v->type = (ViolationType)type;
        v->location = location;
        strcpy(v->dateTime, dateTime);
        v->fineAmount = fine;
        v->paid = paid;
        v->next = NULL;
        if (violationHead == NULL)
            violationHead = v;
        else
            tail->next = v;
        tail = v;

        if (id >= nextViolationID)
            nextViolationID = id + 1;
    }
    fclose(fp);
}

void loadAllData(){
    loadVehicles();
    loadLocations();
    loadViolations();
    loadPayments();
}

void menu(){
    printf(" TRAFFIC VIOLATION & FINE MANAGEMENT SYSTEM\n");
    printf("1.register Vehicle\n");
    printf("2.record violation\n");
    printf("3.make payment\n");
    printf("4.find owners vehicles and violations\n");
    printf("5.find violations at location\n");
    printf("6.sort vehicles by unpaid violations\n");
    printf("7.sort locations by number of violations\n");
    printf("8.sort locations by Fine Revenue\n");
    printf("9.rank violation types by frequency\n");
    printf("10.rank by repeat-offender percentage\n");
    printf("11.vehicles with violations >= Threshold\n");
    printf("12.show pending violations of Vehicle\n");
    printf("13.sort vehicles by Pending Fine\n");
    printf("14.display Payment History\n");
    printf("15.display All Vehicles\n");
    printf("16.display All Locations\n");
    printf("17.display All Violations\n");
    printf("18.register loation\n");
    printf("0.exit\n\n");
    printf("enter choice: ");
}

int main(){
    int choice;
      loadAllData();
    do
    {   menu();
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                registerVehicle();
                break;
            case 2:
                addViolation();
                break;
            case 3:
            {   int voilationId;
                printf("\nEnter violation ID: ");
                scanf("%d", &voilationId);

                makePayment(voilationId);
                break;
            }
           case 4:
            {   char owner[100];
                printf("Enter owner name: ");
                scanf(" %s",owner);

                ownerVoilations(owner);
                break;
            }
            case 5:
            {    int id;
                printf("\nEnter location ID: ");
                scanf("%d", &id);
                locationViolations(id);
                break;
            }
            case 6:
                sortVehiclesByUnpaidViolations();
                break;
            case 7:
                sortLocationsByCount();
                break;
            case 8:
                sortLocationsByRevenue();
                break;
            case 9:
                violationTypeFrequency();
                break;
            case 10:
                repeatOffenderFractions();
                break;
            case 11:
            {    int threshold;
                printf("\nEnter threshold: ");
                scanf("%d", &threshold);
                vehiclesAtLeastThreshold(threshold);
                break;
            }
            case 12:
                vehiclePendingViolations();
                break;
            case 13:
                sortVehiclesByPendingFine();
                break;
            case 14:
                paymentHistory();
                break;
            case 15:
                displayAllVehicles();
                break;
            case 16:
                displayAllLocations();
                break;
            case 17:
                displayAllViolations();
                break;
             case 18:
                addLocation();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

     saveAllData();
    printf("Data saved successfully.\n");
    return 0;
}