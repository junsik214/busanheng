#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5
#define MOVE_LEFT 1
#define MOVE_STAY 0
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

int input(int min, int max, char* prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        scanf_s("%d", &value);
        if (value >= min && value <= max) {
            break;
        }
    }
    return value;
}

void train_print(int train_length, int citizen_location, int zombie_location, int madongseok_location) {
    for (int i = 1; i <= train_length; i++) {
        printf("#");
    }
    printf("\n#");
    for (int i = 1; i <= (train_length - 2); i++) {
        if (i == citizen_location) {
            printf("C");
        }
        else if (i == zombie_location) {
            printf("Z");
        }
        else if (i == madongseok_location) {
            printf("M");
        }
        else {
            printf(" ");
        }
    }
    printf("#\n");
    for (int i = 1; i <= train_length; i++) {
        printf("#");
    }
    printf("\n\n");
}

int main() {
    srand(time(NULL));

    // �ʱ� ���� scanf
    int train_length = input(LEN_MIN, LEN_MAX, "train length(15~50)>> ");
    int madongseok_stamina = input(STM_MIN, STM_MAX, "madongseok stamina(0~5)>> ");
    int p = input(PROB_MIN, PROB_MAX, "percentile probability 'p'(10~90)>> ");

    // C, Z, M ��ġ, C, M aggro
    int citizen_location = train_length - 6;
    int zombie_location = train_length - 3;
    int madongseok_location = train_length - 2;
    int citizen_aggro = 1, madongseok_aggro = 1;

    int count = 0;
    int zombie_can_move = 1;

    // �ʱ� ���� ���
    train_print(train_length, citizen_location, zombie_location, madongseok_location);

    int c_i = 0, z_i = 0;
    while (1) {
        // �ù� �̵�    |   (100 - p)%Ȯ���� �̵�    |   move = aggro 1 up, stay = aggro 1 down
        if (rand() % 100 < (100 - p)) {
            // �̵�
            citizen_location--;
            c_i = 1;
            if (citizen_aggro < AGGRO_MAX) {
                citizen_aggro++;
                c_i = 2;
            }
        }
        else {
            // ���
            c_i = 3;
            if (citizen_aggro > AGGRO_MIN) {
                citizen_aggro--;
                c_i = 4;
            }
        }

        // ���� �̵�    |    p% Ȯ���� �̵�  |   ��׷� ��ġ C >= M�� �� left, C < M�� �� right   |   �̵� �Ϸ��� ���⿡ ��������� ������    |   �������� ������ ��� ��������
        if (rand() % 100 < p && zombie_can_move == 1) {
            if (count % 2 == 0) {
                if (citizen_aggro >= madongseok_aggro) {
                    z_i = 4;
                    if (abs(citizen_location - zombie_location) != 1) {
                        zombie_location--;
                        z_i = 1;
                    }
                }
                else {
                    z_i = 4;
                    if (abs(madongseok_location - zombie_location) != 1) {
                        zombie_location++;
                        z_i = 2;
                    }
                }
            }
            else {
                z_i = 3;
            }
        }
        else {
            z_i = 4;
            if (count % 2 == 1) z_i = 3;
        }

        // �ù�, ���� �̵� ���
        train_print(train_length, citizen_location, zombie_location, madongseok_location);
        printf("\n");

        switch (c_i) {
        case 1:
            printf("citizen: %d -> %d (aggro %d)\n", citizen_location + 1, citizen_location, citizen_aggro); break;
        case 2:
            printf("citizen: %d -> %d (aggro %d -> %d)\n", citizen_location + 1, citizen_location, citizen_aggro - 1, citizen_aggro); break;
        case 3:
            printf("citizen: stay %d (aggro %d)\n", citizen_location, citizen_aggro); break;
        case 4:
            printf("citizen: stay %d (aggro %d -> %d)\n", citizen_location, citizen_aggro + 1, citizen_aggro); break;
        }

        switch (z_i) {
        case 1:
            printf("zombie: %d -> %d\n", zombie_location + 1, zombie_location); break;
        case 2:
            printf("zombie: %d -> %d\n", zombie_location - 1, zombie_location); break;
        case 3:
            if (zombie_can_move == 0) {
                printf("zombie: stay %d (cannot move), (M caught Z)\n", zombie_location);
            }
            else {
                printf("zombie: stay %d (cannot move)\n", zombie_location);
            }
            break;
        case 4:
            if (zombie_can_move == 0) {
                printf("zombie: stay %d (M caught Z)\n", zombie_location);
            }
            else {
                printf("zombie: stay %d\n", zombie_location);
            }
            break;
        }
        zombie_can_move = 1;
        printf("\n\n");

        // ������ �̵�   |   0: stay |   1: left | ��� = aggro 1 down, �̵� = aggro 1 up
        int madongseok_move;
        int m_i;
        if (abs(madongseok_location - zombie_location) == 1) {
            do {
                printf("madongseok move(0:stay)>> ");
                scanf_s("%d", &madongseok_move);
            } while (madongseok_move != 0);
        }
        else {
            do {
                printf("madongseok move(0:stay, 1:left)>> ");
                scanf_s("%d", &madongseok_move);
            } while (madongseok_move != 1 && madongseok_move != 0);
        }

        if (madongseok_move == MOVE_LEFT) {
            madongseok_location--;
            m_i = 3;
            if (madongseok_aggro < AGGRO_MAX) {
                madongseok_aggro++;
                m_i = 2;
            }
        }
        else {
            m_i = 1;
            if (madongseok_aggro > AGGRO_MIN) {
                m_i = 0;
                madongseok_aggro--;
            }
        }

        // ������ �̵� ���
        train_print(train_length, citizen_location, zombie_location, madongseok_location);

        switch (m_i) {
        case 0:
            printf("madongseok: stay %d(aggro: %d -> %d, stamina: %d)\n", madongseok_location, madongseok_aggro + 1, madongseok_aggro, madongseok_stamina); break;
        case 1:
            printf("madongseok: stay %d(aggro: %d, stamina: %d)\n", madongseok_location, madongseok_aggro, madongseok_stamina); break;
        case 2:
            printf("madongseok: %d -> %d(aggro: %d -> %d, stamina: %d)\n", madongseok_location + 1, madongseok_location, madongseok_aggro - 1, madongseok_aggro, madongseok_stamina); break;
        case 3:
            printf("madongseok: %d -> %d(aggro: %d, stamina: %d)\n", madongseok_location + 1, madongseok_location, madongseok_aggro, madongseok_stamina); break;
        }

        printf("\n");

        ///////////////////////////////////////////////// �ൿ

        // �ù� Ż�� Ȯ�� �� �� �ƹ��͵� ����
        if (citizen_location == 1) {
            printf("YOU WIN!\n");
            return 0;
        }
        else {
            printf("citizen does nothing.\n");
        }

        // ��, ��, �� ���� �پ��ִ� ���
        if (abs(zombie_location - citizen_location) == 1 && (abs)(zombie_location - madongseok_location) == 1) {
            if (citizen_aggro >= madongseok_aggro) {
                printf("Zombie attacked citizen (aggro: %d vs. %d)\n", citizen_aggro, madongseok_aggro);
                printf("GAME OVER! citizen dead...");
                return 0;
            }
            else {
                madongseok_stamina--;
                printf("Zombie attacked madongseok (aggro: %d vs. %d, madongseok stamina: %d -> %d)\n", citizen_aggro, madongseok_aggro, madongseok_stamina + 1, madongseok_stamina);
                if (madongseok_stamina == 0) {
                    printf("GAME OVER! madongseok dead...\n");
                    return 0;
                }
            }
        }// ��, �� �پ��ִ� ���
        else if (abs(zombie_location - citizen_location) == 1) {
            printf("GAME OVER! citizen dead...\n");
            return 0;
        } // ��, �� �پ��ִ� ���
        else if (abs(zombie_location - madongseok_location) == 1) {
            madongseok_stamina--;
            printf("Zombie attacked madongseok (madongseok stamina: %d -> %d)\n", madongseok_stamina + 1, madongseok_stamina);
            if (madongseok_stamina == 0) {
                printf("GAME OVER! madongseok dead...\n");
                return 0;
            }
        } // �ƹ��͵� �Ⱥپ� ����, �ϰ͵� ����
        else {
            printf("zombie attacked nobody.\n");
        }
        printf("\n");

        int madongseok_action = 0;

        // ������ �ൿ   |   0: �޽�(aggro 1 down, stamina 1 up), 1: ����(��׷� �ִ�� ����), 
        // 2: (�����, ����� �پ����� ��� ����, (100 - p)%Ȯ���� ����� �����ϸ� ���� �� ���� �� ������, �̵� �������� ���    |   ���� ���� ������� aggro 2up, stamina 1down
        if (abs(madongseok_location - zombie_location) != 1) {
            do {
                printf("madongseok action(0.rest, 1.provoke)>> ");
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != 0 && madongseok_action != 1);
        }
        else {
            do {
                printf("madgonseok action(0.rest, 1.provoke, 2.pull)>> ");
                scanf_s("%d", &madongseok_action);
            } while (madongseok_action != 0 && madongseok_action != 1 && madongseok_action != 2);
        }

        int m_a = madongseok_aggro, m_s = madongseok_stamina;
        switch (madongseok_action) {
        case 0:
            if (madongseok_aggro != 0)
                madongseok_aggro--;
            if (madongseok_stamina != 5)
                madongseok_stamina++;
            printf("madongseok rests...\n");

            if (m_a != 0 && m_s != 5) {
                printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", madongseok_location, m_a, madongseok_aggro, m_s, madongseok_stamina);
            }
            else if (m_a != 0 && m_s == 5) {
                printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok_location, m_a, madongseok_aggro, madongseok_stamina);
            }
            else if (m_a == 0 && m_s != 5) {
                printf("madongseok: %d (aggro: %d, stamina: %d -> %d)\n", madongseok_location, madongseok_aggro, m_s, madongseok_stamina);
            }
            else {
                printf("madongseok: %d (aggro: %d, stamina: %d)\n", madongseok_location, madongseok_aggro, madongseok_stamina);
            }
            break;
        case 1:
            printf("madongseok provoked zombie...\n");
            if (madongseok_aggro < 5) {
                madongseok_aggro = AGGRO_MAX;
                printf("madongseok: %d (aggro: %d -> %d, stamina: %d)\n", madongseok_location, m_a, madongseok_aggro, madongseok_stamina);
            }
            else {
                printf("madongseok: %d (aggro: %d, stamina: %d)\n", madongseok_location, madongseok_aggro, madongseok_stamina);
            }
            break;
        case 2:
            if (rand() % 100 < (100 - p)) {
                zombie_can_move = 0;
                printf("madongseok pulled zombie... Next turn, it can't move\n");
            }
            else {
                printf("madongseok failed to pull zombie\n");
            }

            printf("\n");
            if (5 < madongseok_aggro + 2) {
                madongseok_aggro = 5;
                if (m_a == madongseok_aggro) {
                    printf("madongseok: %d (aggro: %d, ", madongseok_location, madongseok_aggro);
                }
                else {
                    printf("madongseok: %d (aggro: %d -> %d, ", madongseok_location, m_a, madongseok_aggro);
                }
            }
            else {
                madongseok_aggro = madongseok_aggro + 2;
                printf("madongseok: %d (aggro: %d -> %d, ", madongseok_location, m_a, madongseok_aggro);
            }

            madongseok_stamina--;
            printf("stamina: %d -> %d)\n", m_s, madongseok_stamina);

            if (madongseok_stamina == 0) {
                printf("GAME OVER! madongseok dead...\n");
                return 0;
            }

            break;
        }

        count++;
    }

    return 0;
}