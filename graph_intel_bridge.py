import ctypes
import os
import sys

class GraphIntelBridge:
    def __init__(self):
        if not os.path.exists("./libintel.so") and not os.path.exists("./libintel.dll"):
            if sys.platform.startswith("win"):
                os.system("gcc -shared -o libintel.dll intel_graph.c")
                lib_path = "./libintel.dll"
            else:
                os.system("gcc -shared -fPIC -o libintel.so intel_graph.c")
                lib_path = "./libintel.so"
        else:
            lib_path = "./libintel.dll" if sys.platform.startswith("win") else "./libintel.so"

        self.lib = ctypes.CDLL(lib_path)
        self.lib.init_graph.restype = ctypes.c_void_p
        self.lib.define_entity.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
        self.lib.link_entities.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_char_p]
        self.lib.trace_relationship_chain.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p]
        self.lib.free_graph.argtypes = [ctypes.c_void_p]
        
        self.graph_ptr = self.lib.init_graph()

    def register_node(self, idx: int, name: str):
        self.lib.define_entity(self.graph_ptr, idx, name.encode('utf-8'))

    def create_relation(self, source_id: int, target_id: int, description: str):
        self.lib.link_entities(self.graph_ptr, source_id, target_id, description.encode('utf-8'))

    def compile_graph_pathway(self, entry_id: int) -> str:
        buf = ctypes.create_string_buffer(2048)
        self.lib.trace_relationship_chain(self.graph_ptr, entry_id, buf)
        return buf.value.decode('utf-8')

    def __del__(self):
        if hasattr(self, 'lib') and self.graph_ptr:
            self.lib.free_graph(self.graph_ptr)
