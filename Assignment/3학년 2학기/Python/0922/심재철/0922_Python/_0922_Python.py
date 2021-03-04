
#class MyContextManager():
#    def __init__(self, gener): 
#        self.gener = gener
    
#    def __call__(self, path="test.txt"):        
#        self.path = path
        
#        with self as file:             
#            try:
#                s = file.read()            
#                print(s)
#            except Exception as e:
#                print(e)
            

#    def __enter__(self):
#        self.flie = self.gener(self.path)
#        return self.flie
    
#    def __exit__(self, ext, exv, trb):        
#        self.flie.close()
#        return True


#@MyContextManager
#def open3(path):
#    file = open(path)
#    return file

#open3("test.txt")



class MyContextManager2:
    def __init__(self, gener):
        self.gener = gener        

    def __call__(self, path):
        self.file = self.gener(path)        
        return self

    def __enter__(self):
        return self.file.__next__()

    def __exit__(self, ext, exv, trb):
        return True

@MyContextManager2
def open4(path):    
    file = open(path)
    try:        
        print('yielding file . . .')
        yield file
    finally:        
        print('closing file . . .')
        file.close()


with open4("test.txt") as file:
    s = file.read()
    print(s)

#from contextlib import contextmanager

#@contextmanager
#def open3(path):
#    print('opening file . . .')
#    file = open(path)
#    try:
#        print('yielding file . . .')
#        yield file
#    finally:
#        print('closing file . . .')
#        file.close()

#with open3("test.txt") as file:    
#    print(type(file))
#    s = file.read()    
#    print(s)

