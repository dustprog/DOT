#include <math.h>
#include "../Interface/EntityBase.h"
#include "../Interface/InterfaceBase.h"
#define M_PI           3.14159265358979323846
struct Scoring //Contains a set of helper functions for assisting the entity to figure out what helps it the most in its current eco state
{
    static float Attune(float input) //Is an increase from 30% to 60% worth just as much as an increase from 60% to 90%?
	{
		return 10 / input;
    }
    static float TimeAspect(float inputscore, int Delta, int EntityValue) //How much time is this process going to take? Are we going to die completing it?
	{
		inputscore *= Delta; //How much attribute do we need during this time?
		inputscore = 1 - (inputscore / EntityValue); //The more attribute it takes to complete a task, the lower timeaspect becomes
		return inputscore;
	}
#define SQRT_MAGIC_F 0x5f3759df 
    static float  sqrt2(float x)
	{
		const float xhalf = 0.5f*x;

		union // get bits for floating value
		{
			float x;
			int i;
		} u;
		u.x = x;
		u.i = SQRT_MAGIC_F - (u.i >> 1);  // gives initial guess y0
		return x*u.x*(1.5f - xhalf*u.x*u.x);// Newton step, repeating increases accuracy 
	}
    static float CostAttune_Sqrt(float X)
	{
		return X / (sqrt2(1 + (X * X)));
	}
    static float CostAttune_XToTheFourth(float X, float Factor = 1)
    {
        return 1 - (1 / (1+ Factor*(X*X)));
    }

    static float PositiveScore(float CurrentValue, float FutureValue, float Q1, float Q3, float Max)
    {
        float med = (Q1 + Q3) / 2;

        float CurrentValue_t = CurrentValue - (float)Q3; //Scale the number to a  0 - 2 range
        CurrentValue_t /= med;
        float FutureValue_t = FutureValue - (float)Q3; //Scale the numnber to a 0 - 2 range
        FutureValue_t /= med;

        return CostAttune_XToTheFourth(CurrentValue_t, CurrentValue / Q1) - CostAttune_XToTheFourth(FutureValue_t,FutureValue / Q1); //Weigh the difference, but make sure anything above the third quartile isn't as important
    }
    static float PositiveScore(float CurrentValue, float FutureValue, Gradient grad)
	{
		//This is what a Positive score graph would look like if the sigmoids were drawn out: http://i.imgur.com/fYY55Yv.png
        float med = (grad.Q1 + grad.Q3) / 2;

        float CurrentValue_t = CurrentValue - (float)grad.Q3; //Scale the number to a  0 - 2 range
        CurrentValue_t /= med;
        float FutureValue_t = FutureValue - (float)grad.Q3; //Scale the numnber to a 0 - 2 range
        FutureValue_t /= med;

        return CostAttune_XToTheFourth(CurrentValue_t, CurrentValue / grad.Q1) - CostAttune_XToTheFourth(FutureValue_t,FutureValue / grad.Q1); //Weigh the difference, but make sure anything above the third quartile isn't as important
    }

	//Note that this approximation is VERY VERY crude. Its used for distant scheduling among AIs
    static float PositiveScore_Linear(float CurrentValue, float FutureValue, Gradient grad)
	{
		float Q3_Factor = (float)grad.Q3 / (float)grad.Max;
		Q3_Factor *= 2;

		CurrentValue /= (float)grad.Max; //Scale the number to a  0 - 2 range
		CurrentValue *= 2;

		FutureValue /= (float)grad.Max; //Scale the numnber to a 0 - 2 range
		FutureValue *= 2;

		return (CurrentValue + FutureValue) - Q3_Factor;
	}
	//Note that this approximation is VERY VERY crude. Its used for distant scheduling among AIs
    static float PositiveScore_Linear(float CurrentValue, float FutureValue, float Q3, float Max)
	{
		float Q3_Factor = Q3 / Max;
		Q3_Factor *= 2;

		CurrentValue /= Max; //Scale the number to a  0 - 2 range
		CurrentValue *= 2;

		FutureValue /= Max; //Scale the numnber to a 0 - 2 range
		FutureValue *= 2;

		return (CurrentValue + FutureValue) - Q3_Factor;
	}
    static float NegativeScore(float CurrentValue, float FutureValue, float Q1,float Q3, float Max)
    {

        //This is what a Positive score graph would look like if the sigmoids were drawn out: http://i.imgur.com/fYY55Yv.png
        float med = (Q1 + Q3) / 2;

        float CurrentValue_t = CurrentValue - (float)Q1; //Scale the number to a  0 - 2 range
        CurrentValue_t /= med;
        float FutureValue_t = FutureValue - (float)Q1; //Scale the numnber to a 0 - 2 range
        FutureValue_t /= med;

        return CostAttune_XToTheFourth(CurrentValue_t, CurrentValue / Q3) - CostAttune_XToTheFourth(FutureValue_t,FutureValue / Q3); //Weigh the difference, but make sure anything above the third quartile isn't as important
    }
    static float NegativeScore(float CurrentValue, float FutureValue, Gradient grad)
	{
        float med = (grad.Q1 + grad.Q3) / 2;

        float CurrentValue_t = CurrentValue - (float)grad.Q1; //Scale the number to a  0 - 2 range
        CurrentValue_t /= med;
        float FutureValue_t = FutureValue - (float)grad.Q1; //Scale the numnber to a 0 - 2 range
        FutureValue_t /= med;

        return CostAttune_XToTheFourth(CurrentValue_t, CurrentValue / grad.Q3) - CostAttune_XToTheFourth(FutureValue_t,FutureValue / grad.Q3); //Weigh the difference, but make sure anything above the third quartile isn't as important
    }

	//Note that this approximation is VERY VERY crude. Its used for distant scheduling among AIs
    static float NegativeScore_Linear(float CurrentValue, float FutureValue, Gradient grad)
	{
		float Q1_Factor = (float)grad.Q3 / (float)grad.Max;
		Q1_Factor *= 2;

		CurrentValue /= (float)grad.Max; //Scale the number to a  0 - 2 range
		CurrentValue *= 2;

		FutureValue /= (float)grad.Max; //Scale the numnber to a 0 - 2 range
		FutureValue *= 2;

		return (-CurrentValue - FutureValue) - Q1_Factor;
	}
};
