import os
import sys
import zlib


def topo_order_commits():
    findgit()
    #go to ./git/refs/heads
    os.chdir(os.getcwd()+'/.git/refs/heads')

    global_branches, global_head_to_branches = getbranches(os.getcwd())
    os.chdir('../../')
    global_commit_nodes, global_root_hashes = build_commit_graph(os.getcwd(), global_branches)
    

#
#    for key, value in commit_nodes.items():
#        print('NODE ', key, '->', value, ':')
#        print("PARENTS")
#        for parent in value.parents:
#            print(parent)
#
#        print("CHILDREN")
#        for child in value.children:
#            print(child)
#
#        print('\n')
#
#    print('\n\n')
#
#    print(root_hashes)
    
    
    global_ordered_commits = get_topo_ordered_commits(global_commit_nodes, global_root_hashes)

    print_topo_ordered_commits_with_branch_names(global_commit_nodes, global_ordered_commits, global_head_to_branches)
    

class CommitNode:
    def __init__(self, commit_hash):
          """
          :type commit_hash: str
          """
          self.commit_hash = commit_hash
          self.parents = set()
          self.children = set()



def findgit():
    while(os.getcwd() != '/'):
        path = os.getcwd()
        for file in os.listdir(path):
            if(file == '.git' and os.path.isdir(file)):
                return path
        os.chdir('../')

    sys.stderr.write('Not inside a Git repository')
    exit(1)
    

def getbranches(path):
    branches=[]
    head_to_branches={}
    for file in os.listdir(path):
        if(os.path.isdir(path+'/'+file)):
            getbranches(path+'/'+file)
        else:
            #go inside file and get commit hash
            f = open(path+'/'+file, 'r')
            contents = f.read().strip()
            f.close()
            branches.append(contents)
            
            if(contents in head_to_branches):
                head_to_branches[contents].append(file)
            else:
                head_to_branches[contents] = [file]
    return branches, head_to_branches
        
            

def build_commit_graph(git_dir, local_branch_heads):
    commit_nodes={} #key: commit hash; value: commit node
    root_hashes = set()
    stack = local_branch_heads
    visited=set()  #contains visited commit hashes

    #commit_nodes maps hash to node
    
    while(stack):
        commit_hash = stack.pop()
        if(commit_hash in visited):
            continue
            
        visited.add(commit_hash)
        
        if(commit_hash not in commit_nodes):
            #What do you do if the commit weâ€™re on isnâ€™t in commit_nodes?
            node = CommitNode(commit_hash)
            commit_nodes[commit_hash]=node
            
            
        commit_node = commit_nodes[commit_hash]
        
        #Previously, we only had the hash id of the commit; how do we get the entire node?
        
        #Find commit_hash in the objects folder, decompress it, and get parent commits
        commit_path = git_dir+'/objects/'+commit_hash[:2]+"/"+commit_hash[2:]

        if(os.path.isfile(commit_path)):
            compressed = open(commit_path, 'rb').read()
            decompressed = zlib.decompress(compressed).decode("utf-8")
            decompressed = decompressed.replace('\n',' ')
            filedata = decompressed.split(' ')
            
            
            for i in range(0, len(filedata)-1):
                if(filedata[i]=='parent'):
                    commit_node.parents.add(filedata[i+1])
            
        
        if not commit_node.parents:
        #What list do we add commit_hash to if it doesnâ€™t have any parent commits?
            root_hashes.add(commit_hash)
            
        for p in commit_node.parents:
            if(p not in visited):
            #What do we do if p isnâ€™t in visited?
                stack.append(p)
                
                
            if(p not in commit_nodes):
            #What do we do if p isnâ€™t in commit_nodes?
                node = CommitNode(p)
                commit_nodes[p] = node

            #how do we record that commit_hash is a child of commit node p?
            commit_nodes[p].children.add(commit_hash)
            
    return commit_nodes, root_hashes

    
    
def get_topo_ordered_commits(commit_nodes, root_hashes):
    order = []
    visited = set()
    temp_stack = []
    stack = sorted(root_hashes)
    
    while(stack):
        v = stack.pop()
        #what do you do if v is already visited?
        
        if(v in visited):
            continue
        visited.add(v)
        
        #while v is not a child of the vertex on top of the temp_stack:
        if(temp_stack):
            while(v not in commit_nodes[temp_stack[-1]].children):
                g = temp_stack.pop()
                order.append(g)
                
        temp_stack.append(v)
        for c in sorted(commit_nodes[v].children):
            #What do you do is c has already been visited?
            if(c in visited):
                continue
            stack.append(c)
            

    #Add the rest of the temp_stack to the order
    while(len(temp_stack)!=0):
        order.append(temp_stack.pop())
    return order
    
    
def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')
        branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')

    
if __name__ == "__main__":
    topo_order_commits()
