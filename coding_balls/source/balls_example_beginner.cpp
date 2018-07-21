#include <boss.hpp>
#include "balls_example.hpp"
#include <resource.hpp>

////////////////////////////////////////////////////////////////////////////////
#define LEVEL_NUMBER LEVEL_BEGINNER

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 0
MISSION_DECLARE("STEP_0")
// 일단 워밍업을 좀 해봅시다.
// 가장 간단하게 보이는 함수를 선택하여 간단한 코드를 달고
// 그 결과를 확인하여 봅시다.

// 먼저 OnCrashWall이라는 함수가 있습니다.
// 공이 벽에 부딪히게 되면 자동으로 호출되는 함수입니다.
// 본 함수에서 아무런 처리를 하지 않게 되면 벽을 통과해 밖으로 나갈 것입니다.
// 왼쪽 벽부터 처리하여 봅시다.
//
// Math그룹의 Math::AbsF라는 함수가 등장합니다.
// 소수값의 부호를 제거하여 양수로 만들서 주는 함수입니다.
// 예를 들어 -1, -2, -1.1, -2.2와 같은 수를 1, 2, 1.1, 2.2로 만들어 주는 겁니다.
// 이미 양수인 1, 2, 3을 넣으면 그대로 1, 2, 3로 나옵니다.
STEP_API(bool OnCrashWall)(Wall wall, Point& vec)
{
    if(wall == Wall::LEFT) // 1) 왼쪽 벽에 부딪혔을 때
    {
        vec.x = Math::AbsF(vec.x); // 2) 진행방향인 값을 무조건 양수로 만들면 오른쪽으로 가게 만들 수 있습니다.
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 1
MISSION_DECLARE("STEP_1")
// 이번에는 프레임처리를 배워보겠습니다.
// 모든 애니메이션은 사실 눈속임입니다.
// 영화처럼 정지장면을 연속적으로 보여줌으로써
// 동작하는 것으로 착각하게 만드는 방법입니다.
// 그 한장의 정지화면을 프레임이라고 부릅니다.
//
// 게임을 개발하다보면 특별히 프레임마다 해야될 일이 생길 수 있습니다.
// 아래 OnTick함수가 프레임처리 함수입니다.
// Tick은 CPU의 규칙적인 전류흐름에서 따온 이름입니다.

// 이제 OnTick함수를 한번 다뤄볼까요?
// 파라미터 vec를 주목해 봅시다. 매 프레임시 진행방향을 받아보고
// 참조형(&) 파라미터이므로 그 값을 바꿀 수도 있습니다.
STEP_API(void OnTick)(float sec, Point& pos, Point& vec)
{
    vec.y += 0.5; // 1) 중력(아래)방향으로 진행방향을 약간 당겨보는 겁니다.
}

// OnCrashWall함수에서 오른쪽 벽도 왼쪽 벽과 마찬가지로 처리합니다.
STEP_API(bool OnCrashWall)(Wall wall, Point& vec)
{
    if(wall == Wall::LEFT)
    {
        vec.x = Math::AbsF(vec.x);
    }
    if(wall == Wall::RIGHT) // 1) 마친가지로 오른쪽벽도 똑같이 처리합니다.
    {
        vec.x = -Math::AbsF(vec.x); // 2) 절대값을 구하는 AbsF함수의 결과에 마이너스부호를 붙이면 되죠.
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
#define STEP_NUMBER 2
MISSION_DECLARE("STEP_2")

STEP_API(bool OnCrashWall)(Wall wall, Point& vec)
{
    if(wall == Wall::LEFT)
    {
        vec.x = Math::AbsF(vec.x);
    }
    if(wall == Wall::TOP)
    {
        vec.y = Math::AbsF(vec.y);
    }
    if(wall == Wall::RIGHT)
    {
        vec.x = -Math::AbsF(vec.x);
    }
    if(wall == Wall::BOTTOM)
    {
        vec.y = -Math::AbsF(vec.y) * 0.95;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
static int gStep = 0;
int BallsExample::Beginner::OnStep(int step)
{
	if(step != -1) gStep = step;
	return 3;
}

void BallsExample::Beginner::OnTick(float sec, Point& pos, Point& vec)
{
    switch(gStep)
	{
	case 2:
	case 1: STEP_API_CALL(1, OnTick)(sec, pos, vec); break;
	case 0: break;
	}
}

bool BallsExample::Beginner::OnCrashWall(Wall wall, Point& vec)
{
    switch(gStep)
	{
	case 2: return STEP_API_CALL(2, OnCrashWall)(wall, vec); break;
	case 1: return STEP_API_CALL(1, OnCrashWall)(wall, vec); break;
	case 0: return STEP_API_CALL(0, OnCrashWall)(wall, vec); break;
	}
	return false;
}
