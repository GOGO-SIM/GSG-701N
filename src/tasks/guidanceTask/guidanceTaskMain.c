#include "gsgTypes.h"
#include "taskMain.h"
#include "FreeRTOS.h"
#include "task.h"

// Proportional Navigation 상수
#define PN_GAIN         3.0       ///< 항법 상수 N (3 ~ 5 권장)
#define MISSILE_SPEED   100.0     ///< 미사일 속도 V (m/s), 실제 값으로 교체

// TASK주기 및 dt 정의
#define GUIDANCE_PERIOD pdMS_TO_TICKS(200)  ///< 200 ms → FreeRTOS tick 수
#define GUIDANCE_DT     (0.2)               ///< 200 ms → 0.2 s


/**
 * 두 3D 벡터 a, b의 차를 계산 (a - b)
 */
static tDVector3 vecSubtract(const tDVector3 a, const tDVector3 b)
{
    tDVector3 resultVec;

    resultVec.x = a.x - b.x;
    resultVec.y = a.y - b.y;
    resultVec.z = a.z - b.z;

    return resultVec;
}

/**
 * 두 3D 벡터 a, b의 외적(Cross Product) 계산
 * -> 결과 벡터는 a, b 벡터가 만드는 평면에 수직
 */
static tDVector3 vecCross(const tDVector3 a, const tDVector3 b)
{
    // 각 성분별 외적 계산
    double crossX = (a.y * b.z) - (a.z * b.y);
    double crossY = (a.z * b.x) - (a.x * b.z);
    double crossZ = (a.x * b.y) - (a.y * b.x);

    tDVector3 resultVec;
    resultVec.x = crossX;
    resultVec.y = crossY;
    resultVec.z = crossZ;

    return resultVec;
}

/**
 * @brief 3D 벡터 v를 스칼라 s만큼 스케일링 (v * s)
 */
static tDVector3 vecScale(const tDVector3 v, const double s)
{
    tDVector3 resultVec;
    resultVec.x = v.x * s;
    resultVec.y = v.y * s;
    resultVec.z = v.z * s;
    return resultVec;
}

void vGuidanceTask(void *pvParameters)
{
    // 초기 LOS 벡터 저장
    tDVector3 losPrev = {0.0, 0.0, 0.0};

    // 첫 실행 시점 기록
    TickType_t xLastWakeTime = xTaskGetTickCount();

    while(1)
    {

        // 최신 LOS 읽기
        tDVector3 losNow = gSeekerData.los;

        // LOS 회전율 = (losNow - losPrev) / dt
        tDVector3 deltaLos = vecSubtract(losNow, losPrev);
        tDVector3 losRate  = vecScale(deltaLos, 1.0 / GUIDANCE_DT);

        // PN 법칙 적용: a_cmd = N * V * (losNow × losRate)
        tDVector3 navTerm = vecCross(losNow, losRate);
        gAccCommand = vecScale(navTerm, PN_GAIN * MISSILE_SPEED);

        // 다음 주기 계산을 위해 이전 LOS 갱신
        losPrev = losNow;

        // 실행시간을 포함해 총 200ms 대기
        vTaskDelayUntil(&xLastWakeTime, GUIDANCE_PERIOD);

    }
}
