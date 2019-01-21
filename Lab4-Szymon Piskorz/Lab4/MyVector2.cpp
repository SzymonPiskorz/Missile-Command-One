//@author Szymon Piskorz
//C00242808
//@date 16/11/2018
//Start time 15:12
//End time 16:10
//Start time 14:20
//End time 14:40
//Estimated time: 100min
//Actual time 80min
//-------------------------------------------------------------
//This program shows different calculations to do with vectors 
//-------------------------------------------------------------

#include "MyVector2.h"

//This function gets the lengh (magnitude) of the given vector
float vectorLength(sf::Vector2f t_vector)
{
	float sumOfSquares = (t_vector.x * t_vector.x ) + (t_vector.y * t_vector.y); //gets the lenght squared of the vector
	const float length = sqrt(sumOfSquares); //gets the square root of the vector which gives the lenght (magnitude)
	return length; //returns the lenght to the main
}

//This function gets the squared lenght(magnitude)
float vectorLengthSquared(sf::Vector2f t_vector)
{
	const float lenghtsqrd = vectorLength(t_vector) * vectorLength(t_vector); //mulitplies the vector lenght by itself to get the lenght squared
	return lenghtsqrd; //returns the lenght squared to the main
}

//This function gets the cross product of 2 different vectors
float vectorCorssProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	//gets the cross product by multipling the first vector.x by second vector.y and minusing from the multipling first vector.y by second vector.x
	const float crossProduct = ((t_vectorA.x*t_VectorB.y) - (t_vectorA.y*t_VectorB.x)); 
	return crossProduct; //returns the cross product to the main
}

//This function gets the dot product of 2 different vectors
float vectorDotProduct(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	//gets the dot product by multipling the x of the first and second vector and adding the muliplied y of the first and second vector
	const float dotProduct = ((t_vectorA.x*t_VectorB.x) + (t_vectorA.y*t_VectorB.y));
	return dotProduct; //returns the dot product to the main
}

//This function gets the angle between 2 different vectors
float vectorAngleBetween(sf::Vector2f t_vectorA, sf::Vector2f t_VectorB)
{
	//get the cos inverse of (dot product of vector A and B divided by the muliplied vector lenght of vector A and B)
	// then muliply it by 180/PI to get the angle between in degrees 
	const float angleBetween = (acosf(vectorDotProduct(t_vectorA, t_VectorB)/(vectorLength(t_vectorA)*vectorLength(t_VectorB))))*(180/PI);
	return angleBetween; //returns the angle between to the main
}

//This functions rotates the vector by 90 degrees
sf::Vector2f vectorRotateBy(sf::Vector2f t_vector, float t_angleRadians)
{
	//To get x of the rotated vector you multiply the x of the vector by cos radians and minus the y of the vector multiplied by sin radians
	const float rotatedVectorX = (t_vector.x*cosf(t_angleRadians)-(t_vector.y*sinf(t_angleRadians)));
	//To get y of the rotated vector you multiply the x of the vector by sin radians and minus the y of the vector multiplied by cos radians
	const float rotatedVectorY = (t_vector.x*sinf(t_angleRadians) + (t_vector.y*cosf(t_angleRadians)));

	return sf::Vector2f(rotatedVectorX, rotatedVectorY); //returns the points of the rotated vector
}

//This function gets the projection of the first vector onto the second vector (answer is parallel to second vector)
sf::Vector2f vectorProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	// to get a part of the projection you get the dot product of the 2 different vectors and divide it by the lenght of the second vector
	const float projection = (vectorDotProduct(t_vector, t_onto) / vectorLength(t_onto));
	// you get the rest of the projection by muliping the first part by the second vector and dividing it by the lenght of the second vector 
	const sf::Vector2f projection1 = (projection * t_onto) / vectorLength(t_onto);
	return projection1; //returns the projection to the main
}

//This function gets the rejection of 2 vectors (abswer is perpendicular to second vector)
sf::Vector2f vectorRejection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	//To get the rejection you minus the vector projection of the first vector onto the second vector from the first vector 
	const sf::Vector2f rejection = t_vector - vectorProjection(t_vector, t_onto);
	return rejection; //returns the rejection to the main
}

//This function gets the scalar projection 
float vectorScalarProjection(sf::Vector2f t_vector, sf::Vector2f t_onto)
{
	// gets the dot product of 2 differnet vectors and divides it by the vector lenght of second vector 
	const float scalarProjection = (vectorDotProduct(t_vector, t_onto) / vectorLength(t_onto));
	return scalarProjection; //returns the scalar projection of the 2 vectors
}

//This function get the unit vector of a vector
sf::Vector2f vectorUnitVector(sf::Vector2f t_vector)
{
	//To get the x of unit vector you divide x of the vector by the lenght of the vector
	const float unitVectorX = t_vector.x / vectorLength(t_vector);
	//To get the x of unit vector you divide y of the vector by the lenght of the vector
	const float unitVectorY = t_vector.y / vectorLength(t_vector);
	return sf::Vector2f(unitVectorX, unitVectorY); //returns the 2 points of the vector to the main
}
