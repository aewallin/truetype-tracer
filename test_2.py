import ttt

#wr = ttt.NGC_Writer()
wr = ttt.SEG_Writer()

# wr.scale = 3
wr.arc = False
wr.conic = False
wr.cubic = False
s3 = ttt.ttt("LT",wr)
print s3
