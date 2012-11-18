import truetypetracer as ttt

t = ttt.ngc("a")
print t

s = ttt.dxf()
print s
s2 = ttt.dxf("a")
f = open('test.dxf', 'w')
f.write(s2)

wr = ttt.NGC_Writer()
s3 = ttt.ttt("a",wr)
print s3


wr = ttt.SEG_Writer()

# wr.scale = 1
wr.arc = False
wr.conic = False
wr.cubic = False
s3 = ttt.ttt("LT",wr)
print s3

