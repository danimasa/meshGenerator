pkg load geometry;

initPoint = [0 0 0];
finalPoint = [0 2 0];
midPoint = [1 1 0];
position = 0.5;

center = [0 1 0];
radius = 1;

toInitPoint = center - initPoint;
toFinalPoint = center - finalPoint;
alfa = vectorAngle3d(toInitPoint, toFinalPoint);
beta = alfa * position;

w = radius * cos(beta);
k = radius * sin(beta);

normalVector = [0 0 -1];
orthoVector = cross(normalVector, toInitPoint);

A = [ orthoVector(1) orthoVector(2) orthoVector(3);
      toFinalPoint(1) toFinalPoint(2) toFinalPoint(3);
      normalVector(1) normalVector(2) normalVector(3)];

vec = [k w 0];
resultVector = A * vec';
r = resultVector + center'