def method2(x_i, n, k):
    # define sort function that we need to use.
    def my_sort(my_list):
        return my_list[0]
    
    # keep sorted inputs in an array with their indices.
    indices = list(range(n))
    x_i_sorted = list(zip(x_i, indices))
    x_i_sorted.sort(key=my_sort)

    # keep inputs with a flag that show these nodes use or not.
    flags = [0 for i in range(n)]
    x_i = [[x_i[i], flags[i]] for i in range(len(x_i))]

    # main body
    index = 0 
    created_nodes = []
    mode = ' '
    for i in range(k):

        # find first index that has flag = 0 that mean this node has not been used yet.
        min_cost = float('inf')
        while x_i[x_i_sorted[index][1]][1] != 0:
            index += 1
            if index == n:
                break

        # mode is 'index' that mean we choose a single node not a subset.
        if index != n:
            min_cost = x_i_sorted[index][0]
            mode = 'index'

        # mode is 'created_nodes' that mean we choose a subset of nodes that have been choosen before.
        my_tuple = []
        for node in created_nodes:
            cost = node[4]
            if cost < min_cost:
                min_cost = cost
                my_tuple = node
                mode = 'created_nodes'
        if my_tuple:
            created_nodes.remove(my_tuple)
            my_tuple[0] += my_tuple[4]
            my_tuple[2] += 1
            my_tuple[3] -= 1
            my_tuple[4] *= (-1)
            if my_tuple[2] != n:
                my_tuple[4] += x_i[my_tuple[2]][0]
                x_i[my_tuple[2]][1] = 1
            else:
                my_tuple[4] = float('inf')
                
            if my_tuple[3] != -1:
                my_tuple[4] += x_i[my_tuple[3]][0]
                x_i[my_tuple[3]][1] = 1
            else:
                my_tuple[4] = float('inf')

        # do processing if we are in mode = 'index'
        if mode == 'index':
            # set a complet tuple of information for choosen nodes.
            x_value = x_i_sorted[index][0]
            x_index = x_i_sorted[index][1]
            x_next = x_index + 1
            x_previous = x_index - 1
            x_cost = -x_value
            x_i[x_index][1] = 1
            if x_next != n:
                x_i[x_next][1] = 1
                x_cost += x_i[x_next][0]
            else:
                x_cost = float('inf')

            if x_previous != -1:
                x_i[x_previous][1] = 1
                x_cost += x_i[x_previous][0]
            else:
                x_cost = float('inf')
            my_tuple = [x_value, x_index, x_next, x_previous, x_cost]


        # check if we have a subset of nodes that have overlap with new node that we want to insert.
        c_mode1 = ''
        c_mode2 = ''
        for node in created_nodes:
            
            # if next of my_tuple has overlap with previous of another nodes/node.
            if my_tuple[2] == node[3]:
                x_value = my_tuple[0] + node[0]
                x_index = my_tuple[1]
                x_next = node[2]
                x_previous = my_tuple[3]
                x_cost = node[4] + my_tuple[4] - x_i[my_tuple[2]][0]
                my_tuple = [x_value, x_index, x_next, x_previous, x_cost]
                node1 = node
                c_mode1 = 'ON'                

            # if previous of my tuple has overlap with next of another nodes/node.
            if my_tuple[3] == node[2]:
                x_value = my_tuple[0] + node[0]
                x_index = my_tuple[1]
                x_next = my_tuple[2]
                x_previous = node[3]            
                x_cost = node[4] + my_tuple[4] - x_i[my_tuple[3]][0]
                my_tuple = [x_value, x_index, x_next, x_previous, x_cost]
                node2 = node
                c_mode2 = 'ON'                
                
        if c_mode1 == 'ON':
            created_nodes.remove(node1)
        if c_mode2 == 'ON':
            created_nodes.remove(node2)


                

        created_nodes.append(my_tuple)
        
        
        
#         print('i : ', i)
#         print('nodes : ', created_nodes)
#         print('x_i : \n', x_i)

    output = 0    
    for node in created_nodes:
        output += node[0]
    output *= 2
    

    
    return output

# main code =============================================
# import our nedded libraries
# fuck you!

# define sort function that we need to use.
# nothing.

# get inputs from user.
n, k = map(int, input().split())
x_i = list(map(int, input().split()))

# # convert location of objects to distances.
x_i = [x_i[i] - x_i[i-1] for i in range(1, n)]
n -= 1

output = method2(x_i, n, k)    
print(output)
# =====================================================