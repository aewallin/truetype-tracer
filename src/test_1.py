import ttt

t1 = ttt.ngc()
t = ttt.ngc("abc")
print t

s = ttt.dxf()
print s
s2 = ttt.dxf("dxf")

wr = ttt.NGC_Writer()
# wr.scale = 3
s3 = ttt.ttt("",wr)
print s3
