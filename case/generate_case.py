import pcbnew
import math
import pandas as pd
    
def cal_die1_cor():
    
    for i in range(96):
        col = i % 12  # Column within the row
        row = i // 12 # Each row contains 12 signals
        
        if row == 0 :
            if col % 2 == 1:
                x = Die1_START_X + 2 * col * Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_VSS.append((x, y))
                
        elif row == 1:
            x = Die1_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
            y = Die1_START_Y + row * Ver_SPACING_Y
            Coordination_Die1_VSS.append((x, y))
            
        elif row == 6:
            if col % 2 == 1:
                x = Die1_START_X + 2 * col * Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_VDD.append((x, y))
        
        elif row == 7:
            x = Die1_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
            y = Die1_START_Y + row * Ver_SPACING_Y
            Coordination_Die1_VDD.append((x, y))
            
        else:
            if row == 2:
                x = Die1_START_X + 2 * col * Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_Signal.append((x, y, 1+col*4))
            elif row == 3:
                x = Die1_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_Signal.append((x, y, 3+col*4))
            elif row == 4:
                x = Die1_START_X + 2 * col * Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_Signal.append((x, y, 2+col*4))
            elif row == 5:
                x = Die1_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
                y = Die1_START_Y + row * Ver_SPACING_Y
                Coordination_Die1_Signal.append((x, y, 4+col*4))

    # special case (left side)
    x = Coordination_Die1_VSS[6][0] - 2 * Hor_SPACING_X 
    y = Coordination_Die1_VSS[6][1]
    Coordination_Die1_VSS.append((x, y))
    # special case (left side)
    x = Coordination_Die1_Signal[12][0] - 2 * Hor_SPACING_X 
    y = Coordination_Die1_Signal[12][1]
    Coordination_Die1_VSS.append((x, y))

    # special case (right side)
    x = Coordination_Die1_Signal[11][0] + 2 * Hor_SPACING_X 
    y = Coordination_Die1_Signal[11][1]
    Coordination_Die1_VSS.append((x, y))
    # special case (right side)
    x = Coordination_Die1_Signal[35][0] + 2 * Hor_SPACING_X 
    y = Coordination_Die1_Signal[35][1] 
    Coordination_Die1_VDD.append((x, y))
    

def cal_die2_cor():
    
    for i in range(96):
        col = i % 12  # Column within the row
        row = i // 12 # Each row contains 12 signals
        
        if row == 0 :
            if col % 2 == 1:
                x = Die2_START_X + 2 * col * Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_VSS.append((x, y))
                
        elif row == 1:
            x = Die2_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
            y = Die2_START_Y + row * Ver_SPACING_Y
            Coordination_Die2_VSS.append((x, y))
            
        elif row == 6:
            if col % 2 == 1:
                x = Die2_START_X + 2 * col * Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_VDD.append((x, y))
        
        elif row == 7:
            x = Die2_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
            y = Die2_START_Y + row * Ver_SPACING_Y
            Coordination_Die2_VDD.append((x, y))
            
        else:
            if row == 2:
                x = Die2_START_X + 2 * col * Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_Signal.append((x, y, 1+col*4))
            elif row == 3:
                x = Die2_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_Signal.append((x, y, 3+col*4))
            elif row == 4:
                x = Die2_START_X + 2 * col * Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_Signal.append((x, y, 2+col*4))
            elif row == 5:
                x = Die2_START_X + 2 * col * Hor_SPACING_X + Hor_SPACING_X
                y = Die2_START_Y + row * Ver_SPACING_Y
                Coordination_Die2_Signal.append((x, y, 4+col*4))

# 創建並放置 Bump
def create_layout(board, fp_name, x, y, bump_name, number):
    # 這裡指定具體的封裝文件路徑
     # 使用庫名和封裝名來加載封裝
    libname = "/home/user/.local/share/kicad/7.0/footprints/INFO_D2D_lib.pretty"  # 庫名
    footprint_name = fp_name  # 封裝名
    
    
    # 從庫中加載封裝
    if footprint_name == "signal":  
        footprint = pcbnew.FootprintLoad(libname, 'signal')
    elif footprint_name == "VDD":
        footprint = pcbnew.FootprintLoad(libname, 'VDD')
    else:
        footprint = pcbnew.FootprintLoad(libname, 'VSS')
    
    
    footprint.SetReference(bump_name)

    # 使用 VECTOR2I 設置位置，將毫米轉換為納米
    position = pcbnew.VECTOR2I(pcbnew.FromMM(x), pcbnew.FromMM(y))
    footprint.SetPosition(position)
    
    # 將新元件添加到板上
    board.Add(footprint)

    # 找到剛剛添加的元件，根據參考名稱找到元件
    added_footprint = board.FindFootprintByReference(bump_name)
    
    if added_footprint:
        # 使用 FindPadByNumber() 來找到指定的焊墊編號，這裡是焊墊 1
        
        pad = added_footprint.FindPadByNumber(1)  # 焊墊編號 1
        if (pad and footprint_name=="VDD"):
            pad.SetPadName("P")  # 設置焊墊名稱
        elif (pad and footprint_name=="VSS"):
            pad.SetPadName("G")  # 設置焊墊名稱
        else:
        	pad.SetPadName(number)

# 取得當前打開的PCB檔案
board = pcbnew.GetBoard()
# Constants for the layout
Triangle_Length = 10  # Length of the triangle side in mm

Ver_SPACING_Y = Triangle_Length/2 # Vertical spacing between signal and signal in mm
Hor_SPACING_X = Triangle_Length*math.cos(math.radians(30))  # Horizontal spacing between col in mm
Die_gap = 3*Hor_SPACING_X  # Gap between Die-1 and Die-2 in mm

Die1_START_X = 15.0  # Starting x coordinate in mm
Die1_START_Y = 10  # Starting y coordinate in mm


Coordination_Die1_Signal = []
Coordination_Die1_VDD = []
Coordination_Die1_VSS= []

Coordination_Die2_Signal = []
Coordination_Die2_VDD = []
Coordination_Die2_VSS= []

# 計算 Die-1 和 Die-2 的 Bump 座標
cal_die1_cor()

Die2_START_X = Coordination_Die1_Signal[35][0] + 2 * Hor_SPACING_X + Die_gap  # Starting x coordinate in mm
Die2_START_Y = Die1_START_Y  # Starting y coordinate in mm

cal_die2_cor()

# 放置 Die-1 的 sginal, VDD, VSS
for x, y, i in Coordination_Die1_Signal:
    create_layout(board, "signal", x, y, f"Die1_Signal_{i}", i)

for i, (x, y) in enumerate(Coordination_Die1_VDD):
    create_layout(board, "VDD", x, y, f"Die1_VDD_{i}", i)

for i, (x, y) in enumerate(Coordination_Die1_VSS):
    create_layout(board, "VSS", x, y, f"Die1_VSS_{i}", i)

# Die-2   
for x, y, i in Coordination_Die2_Signal:
    create_layout(board, "signal", x, y, f"Die2_Signal_{i}", i)

for i, (x, y) in enumerate(Coordination_Die2_VDD):
    create_layout(board, "VDD", x, y, f"Die2_VDD_{i}", i)

for i, (x, y) in enumerate(Coordination_Die2_VSS):
    create_layout(board, "VSS", x, y, f"Die2_VSS_{i}", i)

# 刷新 KiCad 介面顯示
pcbnew.Refresh()

with open("/home/user/tsmc_InFO_D2D/D2D_algorithm/case/d2d_case_bump.location", "w") as f:  # 放大的倍率(kicad*10 = boost)
    min_x = min([point[0] for point in Coordination_Die1_VSS])
    min_y = min([point[1] for point in Coordination_Die1_VSS])
    max_x = max([point[0] for point in Coordination_Die2_VDD])
    max_y = max([point[1] for point in Coordination_Die2_VDD])
    f.write(f"{min_x*10} {min_y*10}\n")
    f.write(f"{max_x*10} {max_y*10}\n")

    for x, y, i in Coordination_Die1_Signal:
        f.write(f"DIE1_SIG_{i} {x * 10} {y * 10} SIG_NET_{i} {0}\n") 
    for i, (x, y) in enumerate(Coordination_Die1_VDD):
        f.write(f"DIE1_VDD_{i} {x * 10} {y * 10} VDD_NET_{i} {1}\n")
    for i, (x, y) in enumerate(Coordination_Die1_VSS):
        f.write(f"DIE1_VSS_{i} {x * 10} {y * 10} VSS_NET_{i} {1}\n")
        
    for x, y, i in Coordination_Die2_Signal:
        f.write(f"DIE2_SIG_{i} {x * 10} {y * 10} SIG_NET_{i} {0}\n")
    for i, (x, y) in enumerate(Coordination_Die2_VDD):
        f.write(f"DIE2_VDD_{i} {x * 10} {y * 10} VDD_NET_{i} {1}\n")
    for i, (x, y) in enumerate(Coordination_Die2_VSS):
        f.write(f"DIE2_VSS_{i} {x * 10} {y * 10} VSS_NET_{i} {1}\n")
f.close()



# exec(open("/home/user/tsmc_InFO_D2D/D2D_algorithm/case/generate_case.py").read())
