

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct Vehicle // Structure for the Vehicle lying on the plane
{
    float x;
    float y;
};

struct CarZone // Structure for the CarZone
{
    struct Vehicle *LocationPoint;
    float AreaSize;
};

struct TrafficGrid // TrafficGrid Structure
{
    struct CarZone *region;
    struct Vehicle **VehiclesInZone; // This is pointer to the Pointer for forming an array

    struct TrafficGrid *North_West; // all four defining a region
    struct TrafficGrid *North_East;
    struct TrafficGrid *South_West;
    struct TrafficGrid *South_East;
};



struct Vehicle *CreateVehicle(float, float);
struct CarZone *new_CarZone(struct Vehicle *, float);
bool vehicles_contained(struct CarZone *, struct Vehicle *);
int ZoneOverlap(struct CarZone *, struct CarZone *);
struct TrafficGrid *TrafficGrid_new(struct CarZone *);
long long Size(struct Vehicle *VehiclesInZone[]);
struct TrafficGrid *subdivision(struct TrafficGrid *);
bool AddToTraffic(struct TrafficGrid *node, struct Vehicle *);
struct Vehicle **SearchTrafficRegion(struct TrafficGrid *root, struct CarZone *);
void DisplayVehicleInfo(struct Vehicle *);
void DisplayVehicleZone(struct CarZone *);





struct CarZone *new_CarZone(struct Vehicle *LocationPoint, float AreaSize) //  This makes the new CarZone and then returns it
{
    struct CarZone *zone = (struct CarZone *)malloc(sizeof(struct CarZone));
    zone->LocationPoint = LocationPoint;
    zone->AreaSize = AreaSize;
    return zone;
}


struct Vehicle *CreateVehicle(float x, float y) //  This will make the new Vehicle from the structure and returns it
{
    struct Vehicle *vehicle = (struct Vehicle *)malloc(sizeof(struct Vehicle));
    vehicle->x = x;
    vehicle->y = y;
    return vehicle;
}

void DisplayVehicleInfo(struct Vehicle *vehicle) // This prints the Vehicle and returns nothing
{
    printf("(%3.3f, %3.3f)\n", vehicle->x, vehicle->y);
}



bool vehicles_contained(struct CarZone *zone, struct Vehicle *vehicle) // This function checks if we insert the Vehicle in the zone or not
{
    
     if (vehicle->x > zone->LocationPoint->x + zone->AreaSize)
    {
        return 0;
    }
    else if (vehicle->x < zone->LocationPoint->x - zone->AreaSize)
    {
        return 0;
    }
    else if (vehicle->y > zone->LocationPoint->y + zone->AreaSize)
    {
        return 0;
    }   
    else if (vehicle->y < zone->LocationPoint->y - zone->AreaSize)
    {
        return 0;
    }
    else
        return 1;
}

void DisplayVehicleZone(struct CarZone *zone) // Prints the region in the terminal in which we can enter the numbers
{
    printf("\n");
    printf("(%3.3f, %3.3f).............(%3.3f, %3.3f)\n", zone->LocationPoint->x - zone->AreaSize, zone->LocationPoint->y + zone->AreaSize, zone->LocationPoint->x + zone->AreaSize, zone->LocationPoint->y + zone->AreaSize);
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("|                                                   |\n");
    printf("(%3.3f, %3.3f).............(%3.3f, %3.3f)\n", zone->LocationPoint->x - zone->AreaSize, zone->LocationPoint->y - zone->AreaSize, zone->LocationPoint->x + zone->AreaSize, zone->LocationPoint->y - zone->AreaSize);
    printf("\n");
}

int ZoneOverlap(struct CarZone *first, struct CarZone *second) // This function
{
    if (first->LocationPoint->x - first->AreaSize < second->LocationPoint->x + second->AreaSize)
    {
        return 1;
    }
    else if (first->LocationPoint->y - first->AreaSize < second->LocationPoint->y + second->AreaSize)
    {
        return 1;
    }
    else if (first->LocationPoint->x + first->AreaSize > second->LocationPoint->x - second->AreaSize)
    {
        return 1;
    }
    else if (first->LocationPoint->y + first->AreaSize > second->LocationPoint->y - second->AreaSize)
    {
        return 1;
    }
    else
        return 0;
}

long long Size(struct Vehicle *VehiclesInZone[]) //  Returns the TrafficGrid Vehicle size
{
    long long count = 0;
    while (count < 4)
    {
        if (VehiclesInZone[count] == NULL)
            return count;
        count++;
    }
    return 4;
}

struct TrafficGrid *TrafficGrid_new(struct CarZone *LocationPoint) // This function forms the new TrafficGrid
{
    struct TrafficGrid *new = (struct TrafficGrid *)malloc(sizeof(struct TrafficGrid));
    new->South_East = NULL;
    new->North_East = NULL;
    new->South_West = NULL;
    new->North_West = NULL;

    new->region = LocationPoint;
    new->VehiclesInZone = (struct Vehicle **)malloc(sizeof(struct Vehicle *) * 4);

    long long counter = 0;
    while (counter < 4) // Assigning all the VehiclesInZone to NULL
    {
        new->VehiclesInZone[counter] = NULL;
        counter++;
    }
    return new;
}








struct Vehicle** SearchTrafficRegion(struct TrafficGrid* root, struct CarZone* range) {
    struct Vehicle** NO = (struct Vehicle*)malloc(sizeof(struct Vehicle) * 500);
    long long index = 0;

    // Early return if regions don't overlap
    if (!ZoneOverlap(root->region, range)) {
        return NO;
    }

    // Check Vehicles directly within the current node's region
    for (int i = 0; i < Size(root->VehiclesInZone); ++i) {
        if (vehicles_contained(range, root->VehiclesInZone[i])) {
            NO[index++] = root->VehiclesInZone[i];
        }
    }

    // Recursively search child nodes if they intersect the range
    struct TrafficGrid* child_nodes[] = {root->North_West, root->South_West, root->North_East, root->South_East};
    for (int i = 0; i < 4; ++i) {
        if (child_nodes[i]) {
            struct Vehicle** child_range = SearchTrafficRegion(child_nodes[i], range);
            while (child_range[index] != NULL) {
                NO[index++] = child_range[index++];
            }
            free(child_range);
        }
    }

    return NO;
}





bool AddToTraffic(struct TrafficGrid *root_node, struct Vehicle *vehicle) // Function to insert the Vehicle to the TrafficGrid
{
    long long Vehicle_Size;

    if (vehicles_contained(root_node->region, vehicle) == 0) // returns 0 if we can't insert the Vehicle in the region
    {
        return 0;
    }
    Vehicle_Size = Size(root_node->VehiclesInZone); // checks the size

    if (root_node->North_West == NULL && Vehicle_Size < 4)
    {
        root_node->VehiclesInZone[Vehicle_Size] = vehicle;
        return 1;
    }

    if (root_node->North_West == NULL)
    {
        subdivision(root_node);
    }

    if (AddToTraffic(root_node->South_West, vehicle)) // Inserts the Vehicle in the region
    {
        return 1;
    }
    else if (AddToTraffic(root_node->North_West, vehicle)) // Inserts the Vehicle in the region
    {
        return 1;
    }
    else if (AddToTraffic(root_node->South_East, vehicle)) // Inserts the Vehicle in the region
    {
        return 1;
    }
    else if (AddToTraffic(root_node->North_East, vehicle)) // Inserts the Vehicle in the region
    {
        return 1;
    }
    else
        return 0;
}

struct TrafficGrid *subdivision(struct TrafficGrid *root) // add the new CarZone of TrafficGrid to the parent root
{
    float size = root->region->AreaSize / 2;

    struct Vehicle *toprightpoint = CreateVehicle(root->region->LocationPoint->x + size, root->region->LocationPoint->y + size);
    root->North_East = TrafficGrid_new(new_CarZone(toprightpoint, size));

    struct Vehicle *topleftpoint = CreateVehicle(root->region->LocationPoint->x - size, root->region->LocationPoint->y + size);
    root->North_West = TrafficGrid_new(new_CarZone(topleftpoint, size));

    struct Vehicle *bottomrightpoint = CreateVehicle(root->region->LocationPoint->x + size, root->region->LocationPoint->y - size);
    root->South_East = TrafficGrid_new(new_CarZone(bottomrightpoint, size));

    struct Vehicle *bottomleftpoint = CreateVehicle(root->region->LocationPoint->x - size, root->region->LocationPoint->y - size);
    root->South_West = TrafficGrid_new(new_CarZone(bottomleftpoint, size));

    

    return root;
}

long long main()
{
    printf("\033[1m\033[4mTraffic Grid\033[0m\n\n");

    printf("Specify Initial Axis Aligned Bounding Box \n");
    printf("Center: \n");
    float x, y;
    printf("X: ");
    scanf("%e", &x); // scanning the x of xy plane
    printf("Y: ");
    scanf("%e", &y); // scanning the y of xy plane
    float X = x, Y = y;
    float AreaSize_xy_plane;
    printf("Side Length of the Square region: ");
    scanf("%e", &AreaSize_xy_plane); // scanning for the Dimensions
    AreaSize_xy_plane = AreaSize_xy_plane / 2;

    struct Vehicle *center = CreateVehicle(x, y);                     // assigning a new Vehicle
    struct CarZone *Region = new_CarZone(center, AreaSize_xy_plane); // assigning a new CarZone
    printf("Traffic Grid Region\n");

    DisplayVehicleZone(Region); // This will print the region in which we are going to insert Vehicles

    struct TrafficGrid *TRAFFICGRID = TrafficGrid_new(Region); // assigning a new TrafficGrid

    long long count;
    printf("Enter Number of Vehicles you want to insert: ");
    scanf("%d", &count); // Scans the number of Vehicles user wants to enter
    int counter = count;
    long long i = 0;

    for (int j = 0; j < counter; j++) // loop for inserting an element in the TrafficGrid
    {
        i++;
        printf("Vehicle %d\n", j + 1);
        float x, y;
        printf("X: ");
        scanf("%e", &x);
        printf("Y: ");
        scanf("%e", &y);

        if (X + AreaSize_xy_plane < x || X - AreaSize_xy_plane > x || Y + AreaSize_xy_plane < y || Y - AreaSize_xy_plane > y) // The Vehicle will not insert if the Vehicle is not in the user-defined region
        {
            printf("Vehicle is Outside Region, So not Inserted\n");
            continue;
        }
        struct Vehicle *k = CreateVehicle(x, y);
        printf("Vehicle: ");
        DisplayVehicleInfo(k);
        if (AddToTraffic(TRAFFICGRID, k) == 0) //  This will insert the element in the TrafficGrid
        {
            printf("Vehicle is Outside Region, So not Inserted\n");
        }
        else
        {
            printf("Vehicle inserted successfully\n");
        }
    }

    printf("\n\033[1m\033[4mVehicles in the search region\033[0m\n\n");
    printf("Specify the centre of the search area\n");
    printf("Center: \n");
    float x_range, y_range;
    printf("X: ");
    scanf("%e", &x_range);
    printf("Y: ");
    scanf("%e", &y_range);

    float AreaSize_xy_plane_range;
    printf("Dimension of the region you want to see the vehicles if it is present or not:(this is the length of side of the square within which you want to check whether points lie.) ");
    scanf("%e", &AreaSize_xy_plane_range);
    AreaSize_xy_plane_range /= 2;

    struct Vehicle *center_range = CreateVehicle(x_range, y_range);
    struct CarZone *Region_range = new_CarZone(center_range, AreaSize_xy_plane_range);
    printf("Search Area Region\n");
    DisplayVehicleZone(Region_range);
    struct Vehicle **res = SearchTrafficRegion(TRAFFICGRID, Region_range); // forms the array present in the given region
    printf("Vehicles inside that Region are as follows(Please note that the programme may return null(0.000,0.000) if there are no points or only one point , by default)\n");
    long long j = 0;
    

    while (res[j] != NULL && j < 500) // loop for printing the element in the given region
    {
        DisplayVehicleInfo(res[j]);
        j++;
    }
    return 0;
}