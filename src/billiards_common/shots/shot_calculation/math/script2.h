//
// Created by thallock on 10/7/21.
//

#ifndef IDEA_SCRIPT2_H
#define IDEA_SCRIPT2_H


/*
 *
# Rolling glance direction.
# Using the model in https://billiards.colostate.edu/technical_proofs/new/TP_A-4.pdf

P.<x, y> = PolynomialRing(QQ, 2)
# The object ball is centered at the origin
ax, ay = var('ax ay') # Location of the current cue position
px, py = var('px py') # Location of the pocket
r1 = var('r1') # The sum of radius of the cue ball
r2 = var('r2') # The sum of radius of the object ball
alpha = 2/7


# The tangent direction must be orthogonal to (x, y)
tx = y; ty = -x;
# tx = -y; ty = x;

# The desired cue location is along a line that is orthogonal to the pocket at some point dx, dy
 # eq_d = (dx - x) * (dx - px) + (dy - y) * (dy - py) == 0
dx, dy = var('dx dy')
dxdy_sol = solve([dx - x == dy - py, dy - y == -(dx - px)], dx, dy)[0]
# dxdy_sol = solve([dx - x == -(dy - py), dy - y == dx - px], dx, dy)[0]
dx = dxdy_sol[0].right(); dy = dxdy_sol[1].right()

# We need the location of the glance to be at radius r1 + r2 from the object ball
eq_r1 = x^2 + y^2 == (r1 + r2)^2
poly_r1 = (eq_r1 - eq_r1.right()).left()

# Let s1 be a distance travelled along the aim line such that it is orthogonal to the tangent line:
s1 = var('s1')
eq_orth = (s1 * ax) * (s1 * ax - tx) + (s1 * ay) * (s1 * ay - ty) == 0
s1 = solve(eq_orth / s1, s1)[0].right()

# The rolling glance means that the direction from the glance point satisfies
s = var('s')
eq_dx = s * dx == (1 - alpha) * tx + alpha * s1 * ax
eq_dy = s * dy == (1 - alpha) * ty + alpha * s1 * ay

s_sol = solve(eq_dx, s)[0].right()
eq_d = (eq_dy.substitute(s=s_sol) * s_sol.denominator()).simplify_rational().expand()
poly_d = (eq_d - eq_d.right()).left().factor()

# If we travel a distance s1 along the dx, dy from x,y, we must be at distance r1 from the pocket
eq_r2 = (dx - px)^2 + (dy - py)^2 == r1^2
poly_r2 = (eq_r2 - eq_r2.right()).left()

# Variables: x, y
# Equations: poly_r1 == 0, poly_r2 == 0, poly_d == 0,

r1 = poly_r1.resultant(poly_r2, y)
r2 = r1.resultant(poly_d, y)
poly = r2.factor()

factor = poly.operands()[0].operands()[0]

(factor^2 / 16 - r2).is_zero()

poly = r3.expand()







r3 = r2.resultant(r1, s1)
r3 = poly_or.resultant(r1, s1)
r4 = r3.resultant(r3, y)

r1 = poly_or.resultant(poly_dx, s1) # x, y, s2, s3
r2 = poly_or.resultant(poly_dy, s1) # x, y, s2, s3
r3 = r1.resultant(r2, s3) # x, y, s2
r4 = poly_r1.resultant(poly_r2, s3) # x, y
r5 = r3.resultant(r4, s2) # x, y


r1 = poly_dx.resultant(poly_dy, s3) # x, y, s1, s2
r2 = poly_r1.resultant(poly_r2, s3)
r3 = r2.resultant(r1, s2)
r4 = poly_or.resultant(r1, s2)
r5 = r3.resultant(r4, s1)
r6 = poly_r1.resultant(r5, y)


r3


r3.resultant(r4, y)

 eq_orth = (s1 * ax) * (s1 * ax - s2 * tx) + (s1 * ay) * (s1 * ay - s2 * ty) == 0

# The rolling glance means that the direction from the glance point satisfies
eq_dx = s3 * dx == (1 - alpha) * s2 * tx + alpha * s1 * ax
eq_dy = s3 * dy == (1 - alpha) * s2 * ty + alpha * s1 * ay


 eq_r2.resultant(
eq_orth.resultant(


dest_eqs = [eq_dx, eq_dy]
for i in range(len(dest_eqs)):
	eq = dest_eqs[i]
	eq = eq.substitute(s=s_sol)
	eq = eq * s_sol.denominator()
	eq = eq.simplify_rational()
	eq = eq - eq.right()
	eq = eq.left()
	eq = eq.expand()
	dest_eqs[i] = eq

t_sol = solve(dest_eqs[0] - dest_eqs[1], t)[0].right()
for i in range(2):
	eq = dest_eqs[i]
	eq = eq.substitute(t=t_sol)
	eq = eq * t_sol.denominator()
	eq = eq.simplify_rational()
	eq = eq.expand()
	dest_eqs[i] = eq

# We need to solve for x, y, dx, dy, s, t
# First, we solve for s. One solution is s = 0, so we divide it out.



expr1 = eq_r2;
expr1 = expr1 - expr1.right()
expr1 = expr1.left()
expr1 = expr1.expand()

expr2 = eq_o
expr2 = eq_o.left()
expr2 = expr2.expand()
dx_sol = solve(expr1 - expr2 == 0, dx)[0].right()
expr1.substitute(dx=dx_sol)
dx_num = dx_sol.numerator()
dx_denom = dx_sol.denominator()
(dx_sol * (px^2 + py^2 - 2*px*x + x^2 - 2*py*y + y^2) * (px - x) - dx_num).is_zero()
















t = var('t')

p1 = t * w * x + w^2 - 2 + y * z
p2 = y^2 - 2 * x^2 + 4 * y * z - w * t
p3 = x * y - w * z + 13 * w^2

r1 = p1.resultant(p2, x)
r2 = p3.resultant(p2, x)

r3 = r1.resultant(r2, y)




eq_o = (dx - x) * (dx - px) + (dy - y) * (dy - py) == 0


P.<x, y, s1, s2, s3> = PolynomialRing(QQ, 5)
r1, r2 = var('r1 r2')
px, py = var('px py')
ax, ay = var('ax ay')
tx = y
ty = -x
alpha = 2/7
dx = x - px
dy = px - y


eq_r1 = x^2 + y^2 - (r1 + r2)^2
eq_r2 = (dx - px)^2 + (dy - py)^2 - r1^2


eq_s = (s1 * ax) * (s1 * ax - s2 * tx) + (s1 * ay) * (s1 * ay - s2 * ty)
eq_dx = (1 - alpha) * tx + alpha * s2 * ax - s3 * (dx - x)
eq_dy = (1 - alpha) * ty + alpha * s2 * ay - s3 * (dy - y)



r1 = eq_dx.resultant(eq_dy, s3)
r2 = r1.resultant(eq_s, s1)
r3 = r2.resultant(eq_r2, s2)
r4 = r3.resultant(eq_r1, y)





poly = r3.factor()
p = 49*ax^4*px^2*r1^2 - 28*ax^3*ay*px^2*r1^2 + 74*ax^2*ay^2*px^2*r1^2 - 20*ax*ay^3*px^2*r1^2 + 25*ay^4*px^2*r1^2 + 98*ax^4*px*py*r1^2 + 132*ax^2*ay^2*px*py*r1^2 + 50*ay^4*px*py*r1^2 + 49*ax^4*py^2*r1^2 + 28*ax^3*ay*py^2*r1^2 + 74*ax^2*ay^2*py^2*r1^2 + 20*ax*ay^3*py^2*r1^2 + 25*ay^4*py^2*r1^2 - 49*ax^4*r1^4 + 28*ax^3*ay*r1^4 - 74*ax^2*ay^2*r1^4 + 20*ax*ay^3*r1^4 - 25*ay^4*r1^4 + 98*ax^4*px^2*r1*r2 - 56*ax^3*ay*px^2*r1*r2 + 148*ax^2*ay^2*px^2*r1*r2 - 40*ax*ay^3*px^2*r1*r2 + 50*ay^4*px^2*r1*r2 + 196*ax^4*px*py*r1*r2 + 264*ax^2*ay^2*px*py*r1*r2 + 100*ay^4*px*py*r1*r2 + 98*ax^4*py^2*r1*r2 + 56*ax^3*ay*py^2*r1*r2 + 148*ax^2*ay^2*py^2*r1*r2 + 40*ax*ay^3*py^2*r1*r2 + 50*ay^4*py^2*r1*r2 - 196*ax^4*r1^3*r2 + 112*ax^3*ay*r1^3*r2 - 296*ax^2*ay^2*r1^3*r2 + 80*ax*ay^3*r1^3*r2 - 100*ay^4*r1^3*r2 + 49*ax^4*px^2*r2^2 - 28*ax^3*ay*px^2*r2^2 + 74*ax^2*ay^2*px^2*r2^2 - 20*ax*ay^3*px^2*r2^2 + 25*ay^4*px^2*r2^2 + 98*ax^4*px*py*r2^2 + 132*ax^2*ay^2*px*py*r2^2 + 50*ay^4*px*py*r2^2 + 49*ax^4*py^2*r2^2 + 28*ax^3*ay*py^2*r2^2 + 74*ax^2*ay^2*py^2*r2^2 + 20*ax*ay^3*py^2*r2^2 + 25*ay^4*py^2*r2^2 - 294*ax^4*r1^2*r2^2 + 168*ax^3*ay*r1^2*r2^2 - 444*ax^2*ay^2*r1^2*r2^2 + 120*ax*ay^3*r1^2*r2^2 - 150*ay^4*r1^2*r2^2 - 196*ax^4*r1*r2^3 + 112*ax^3*ay*r1*r2^3 - 296*ax^2*ay^2*r1*r2^3 + 80*ax*ay^3*r1*r2^3 - 100*ay^4*r1*r2^3 - 49*ax^4*r2^4 + 28*ax^3*ay*r2^4 - 74*ax^2*ay^2*r2^4 + 20*ax*ay^3*r2^4 - 25*ay^4*r2^4 - 98*ax^4*px*r1^2*x - 132*ax^2*ay^2*px*r1^2*x - 50*ay^4*px*r1^2*x + 42*ax^4*py*r1^2*x - 56*ax^3*ay*py*r1^2*x + 132*ax^2*ay^2*py*r1^2*x - 40*ax*ay^3*py*r1^2*x + 90*ay^4*py*r1^2*x - 196*ax^4*px*r1*r2*x - 264*ax^2*ay^2*px*r1*r2*x - 100*ay^4*px*r1*r2*x + 84*ax^4*py*r1*r2*x - 112*ax^3*ay*py*r1*r2*x + 264*ax^2*ay^2*py*r1*r2*x - 80*ax*ay^3*py*r1*r2*x + 180*ay^4*py*r1*r2*x - 98*ax^4*px*r2^2*x - 132*ax^2*ay^2*px*r2^2*x - 50*ay^4*px*r2^2*x + 42*ax^4*py*r2^2*x - 56*ax^3*ay*py*r2^2*x + 132*ax^2*ay^2*py*r2^2*x - 40*ax*ay^3*py*r2^2*x + 90*ay^4*py*r2^2*x - 74*ax^4*px^2*x^2 + 48*ax^3*ay*px^2*x^2 - 148*ax^2*ay^2*px^2*x^2 + 48*ax*ay^3*px^2*x^2 - 74*ay^4*px^2*x^2 - 48*ax^4*px*py*x^2 + 48*ay^4*px*py*x^2 - 74*ax^4*py^2*x^2 - 48*ax^3*ay*py^2*x^2 - 148*ax^2*ay^2*py^2*x^2 - 48*ax*ay^3*py^2*x^2 - 74*ay^4*py^2*x^2 + 32*ax^4*r1^2*x^2 - 48*ax^3*ay*r1^2*x^2 + 16*ax^2*ay^2*r1^2*x^2 - 48*ax*ay^3*r1^2*x^2 - 16*ay^4*r1^2*x^2 + 64*ax^4*r1*r2*x^2 - 96*ax^3*ay*r1*r2*x^2 + 32*ax^2*ay^2*r1*r2*x^2 - 96*ax*ay^3*r1*r2*x^2 - 32*ay^4*r1*r2*x^2 + 32*ax^4*r2^2*x^2 - 48*ax^3*ay*r2^2*x^2 + 16*ax^2*ay^2*r2^2*x^2 - 48*ax*ay^3*r2^2*x^2 - 16*ay^4*r2^2*x^2 + 48*ax^4*px*x^3 - 48*ay^4*px*x^3 + 8*ax^4*py*x^3 + 96*ax^3*ay*py*x^3 + 16*ax^2*ay^2*py*x^3 + 96*ax*ay^3*py*x^3 + 8*ay^4*py*x^3 - 8*ax^4*x^4 - 16*ax^2*ay^2*x^4 - 8*ay^4*x^4
(p^2 * 38416 * (ax^2 + ay^2)^4 - poly).expand().is_zero()

 (p^2 * 38416 * (ax^2 + ay^2)^4 - poly).expand()


 (49*ax^4*px^2*r1^2 - 28*ax^3*ay*px^2*r1^2 + 74*ax^2*ay^2*px^2*r1^2 - 20*ax*ay^3*px^2*r1^2 + 25*ay^4*px^2*r1^2 + 98*ax^4*px*py*r1^2 + 132*ax^2*ay^2*px*py*r1^2 + 50*ay^4*px*py*r1^2 + 49*ax^4*py^2*r1^2 + 28*ax^3*ay*py^2*r1^2 + 74*ax^2*ay^2*py^2*r1^2 + 20*ax*ay^3*py^2*r1^2 + 25*ay^4*py^2*r1^2 - 49*ax^4*r1^4 + 28*ax^3*ay*r1^4 - 74*ax^2*ay^2*r1^4 + 20*ax*ay^3*r1^4 - 25*ay^4*r1^4 + 98*ax^4*px^2*r1*r2 - 56*ax^3*ay*px^2*r1*r2 + 148*ax^2*ay^2*px^2*r1*r2 - 40*ax*ay^3*px^2*r1*r2 + 50*ay^4*px^2*r1*r2 + 196*ax^4*px*py*r1*r2 + 264*ax^2*ay^2*px*py*r1*r2 + 100*ay^4*px*py*r1*r2 + 98*ax^4*py^2*r1*r2 + 56*ax^3*ay*py^2*r1*r2 + 148*ax^2*ay^2*py^2*r1*r2 + 40*ax*ay^3*py^2*r1*r2 + 50*ay^4*py^2*r1*r2 - 196*ax^4*r1^3*r2 + 112*ax^3*ay*r1^3*r2 - 296*ax^2*ay^2*r1^3*r2 + 80*ax*ay^3*r1^3*r2 - 100*ay^4*r1^3*r2 + 49*ax^4*px^2*r2^2 - 28*ax^3*ay*px^2*r2^2 + 74*ax^2*ay^2*px^2*r2^2 - 20*ax*ay^3*px^2*r2^2 + 25*ay^4*px^2*r2^2 + 98*ax^4*px*py*r2^2 + 132*ax^2*ay^2*px*py*r2^2 + 50*ay^4*px*py*r2^2 + 49*ax^4*py^2*r2^2 + 28*ax^3*ay*py^2*r2^2 + 74*ax^2*ay^2*py^2*r2^2 + 20*ax*ay^3*py^2*r2^2 + 25*ay^4*py^2*r2^2 - 294*ax^4*r1^2*r2^2 + 168*ax^3*ay*r1^2*r2^2 - 444*ax^2*ay^2*r1^2*r2^2 + 120*ax*ay^3*r1^2*r2^2 - 150*ay^4*r1^2*r2^2 - 196*ax^4*r1*r2^3 + 112*ax^3*ay*r1*r2^3 - 296*ax^2*ay^2*r1*r2^3 + 80*ax*ay^3*r1*r2^3 - 100*ay^4*r1*r2^3 - 49*ax^4*r2^4 + 28*ax^3*ay*r2^4 - 74*ax^2*ay^2*r2^4 + 20*ax*ay^3*r2^4 - 25*ay^4*r2^4 - 98*ax^4*px*r1^2*x - 132*ax^2*ay^2*px*r1^2*x - 50*ay^4*px*r1^2*x + 42*ax^4*py*r1^2*x - 56*ax^3*ay*py*r1^2*x + 132*ax^2*ay^2*py*r1^2*x - 40*ax*ay^3*py*r1^2*x + 90*ay^4*py*r1^2*x - 196*ax^4*px*r1*r2*x - 264*ax^2*ay^2*px*r1*r2*x - 100*ay^4*px*r1*r2*x + 84*ax^4*py*r1*r2*x - 112*ax^3*ay*py*r1*r2*x + 264*ax^2*ay^2*py*r1*r2*x - 80*ax*ay^3*py*r1*r2*x + 180*ay^4*py*r1*r2*x - 98*ax^4*px*r2^2*x - 132*ax^2*ay^2*px*r2^2*x - 50*ay^4*px*r2^2*x + 42*ax^4*py*r2^2*x - 56*ax^3*ay*py*r2^2*x + 132*ax^2*ay^2*py*r2^2*x - 40*ax*ay^3*py*r2^2*x + 90*ay^4*py*r2^2*x - 74*ax^4*px^2*x^2 + 48*ax^3*ay*px^2*x^2 - 148*ax^2*ay^2*px^2*x^2 + 48*ax*ay^3*px^2*x^2 - 74*ay^4*px^2*x^2 - 48*ax^4*px*py*x^2 + 48*ay^4*px*py*x^2 - 74*ax^4*py^2*x^2 - 48*ax^3*ay*py^2*x^2 - 148*ax^2*ay^2*py^2*x^2 - 48*ax*ay^3*py^2*x^2 - 74*ay^4*py^2*x^2 + 32*ax^4*r1^2*x^2 - 48*ax^3*ay*r1^2*x^2 + 16*ax^2*ay^2*r1^2*x^2 - 48*ax*ay^3*r1^2*x^2 - 16*ay^4*r1^2*x^2 + 64*ax^4*r1*r2*x^2 - 96*ax^3*ay*r1*r2*x^2 + 32*ax^2*ay^2*r1*r2*x^2 - 96*ax*ay^3*r1*r2*x^2 - 32*ay^4*r1*r2*x^2 + 32*ax^4*r2^2*x^2 - 48*ax^3*ay*r2^2*x^2 + 16*ax^2*ay^2*r2^2*x^2 - 48*ax*ay^3*r2^2*x^2 - 16*ay^4*r2^2*x^2 + 48*ax^4*px*x^3 - 48*ay^4*px*x^3 + 8*ax^4*py*x^3 + 96*ax^3*ay*py*x^3 + 16*ax^2*ay^2*py*x^3 + 96*ax*ay^3*py*x^3 + 8*ay^4*py*x^3 - 8*ax^4*x^4 - 16*ax^2*ay^2*x^4 - 8*ay^4*x^4)^2


 */

#endif //IDEA_SCRIPT2_H
