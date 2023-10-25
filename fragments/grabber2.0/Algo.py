lst = [3, 5]        # Input list of stops in route

lst3 = [-3, -4, -5]             # Non-Duplicate values

dict1 = {-3:10, -4:20, -5:60}   # Corresponding paths
lst_paths = []                  # List of Paths in the route

cmd1 = [1, 7, 5, 6]             # Path №10
cmd2 = [1, 7, 4, 7, 4, 7, 6]    # Common part of paths №20-90
S1 = [0, 4, 3, 7, 6]            # Starting Path
F1 = [1, 7, 4, 2, 7, 8]         # Finishing Path

lst_out = []                    # Output list of actions

Start = lst[0]                  # First stop in route
Finish = lst[-1]                # Last stop in route

def add_path(lst1):          # Add a path to the output list
    for i in range(0, len(lst1)): 
        lst_out.append(lst1[i]) 

cmd_S = list(S1)
if Start > 3:
    cmd_S[2] = 2
    while Start > 4:
        cmd_S.insert(1, 5)
        Start -= 1
else: 
    while Start > 1:
        cmd_S.insert(1, 5)
        Start -= 1

cmd_F = list(F1)
if Finish > 3:
    cmd_F[3] = 3
    while Finish > 4:
        cmd_F.insert(5, 5)
        Finish -= 1
else:
    while Finish > 1:
        cmd_F.insert(5, 5)
        Finish -= 1
    
add_path(cmd_S)

for i in range(1, len(lst)):          # Iterate from the 2nd to the last element of lst
    n = lst[i-1] - lst[i]             # Get the difference between each pair of consecutive stops (path ID)                
    for a in range(0, len(lst3)):             # Check for the non-duplicating path ID-sВ
        if n == lst3[a] or n == -1 * lst3[a]:
            lst_paths.append(dict1[lst3[a]])  # Add the path to the paths list
    if n == 1:  # Check for the duplicating path ID-s and sort them out
        if lst[i-1] == 2 or lst[i] == 2:
            lst_paths.append(40)
        elif lst[i-1] == 5 or lst[i] == 5:
            lst_paths.append(30)
        else:
            lst_paths.append(70)
    elif n == -1:
        if lst[i-1] == 2 or lst[i] == 2:
            lst_paths.append(30)
        elif lst[i-1] == 5 or lst[i] == 5:
            lst_paths.append(40)
        else:
            lst_paths.append(70)
    elif n == 2:
        if lst[i-1] == 1 or lst[i] == 1:
            lst_paths.append(90)
        elif lst[i-1] == 6 or lst[i] == 6:
            lst_paths.append(80)
        else:
            lst_paths.append(50)
    elif n == -2:
        if lst[i-1] == 1 or lst[i] == 1:
            lst_paths.append(80)
        elif lst[i-1] == 6 or lst[i] == 6:
            lst_paths.append(90)
        else:
            lst_paths.append(50)

for b in range(0, len(lst_paths)):   # Iterate through lst_paths
    cmd = lst_paths[b]               # Get the current path
    if cmd == 10:  # Check for path №10
        add_path(cmd1)
    else:  # Sort out the other paths by adding the unique elements to the common part
        cmd_temp = list(cmd2)
        if cmd == 20 or cmd == 60:
            cmd_temp.insert(3, 3)
            cmd_temp.insert(6, 2)
        elif cmd == 30 or cmd == 80:
            cmd_temp.insert(3, 3)
            cmd_temp.insert(6, 3)
        elif cmd == 40 or cmd == 90:
            cmd_temp.insert(3, 2)
            cmd_temp.insert(6, 2)
        elif cmd == 50 or cmd == 70:
            cmd_temp.insert(3, 2)
            cmd_temp.insert(6, 3)
        if cmd > 50:
            cmd_temp.insert(5, 5)
        add_path(cmd_temp)
add_path(cmd_F)  # Add the finish path to the paths list

print(len(lst_out))
print(lst_out)