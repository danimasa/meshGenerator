p23 = [0.3421000238 0.1409523810 2.625];
p31 = [0 0.37 2.625];

init_point = p23;
final_point = p31;
length = 0.4365889727;
mid_point = [0.2058489969 0.3074511188 2.625];
init_vector = [-0.380952381 0.9245946590 0.2696692822e-15];
final_vector = [-1 -0.4719212438e-15 -0.2696692822e-15];

d = norm(p31 - p23, 2);

plane_vector1 = init_point - final_point;
plane_vector2 = init_point - mid_point;
plane_point = init_point;
plane_normal = cross(plane_vector1, plane_vector2);

mat = [init_vector; plane_vector1; plane_vector2];
[L, U] = lu(mat);

mat2 = [final_vector; plane_vector1; plane_vector2];
[L2, U2] = lu(mat2);

toCenter1 = cross(init_vector, plane_normal);
toCenter2 = cross(final_vector, plane_normal);

A = [ toCenter1(1), toCenter2(1) * -1; toCenter1(2), toCenter2(2) * -1];
b = [ final_point(1) - init_point(1); final_point(2) - init_point(2)];

alphaBeta = A \ b;

alfa = alphaBeta(1);
xc = init_point(1) + alfa * toCenter1(1);
yc = init_point(2) + alfa * toCenter1(2);
zc = init_point(3) + alfa * toCenter1(3);

c_point = [xc yc zc];
radius = norm(init_point - c_point, 2);

toInitPoint = c_point - init_point;
toFinalPoint = c_point - final_point;

angle = acos(dot(toInitPoint, toFinalPoint) / (norm(toInitPoint) * norm(toFinalPoint)));
arcLength = radius * angle