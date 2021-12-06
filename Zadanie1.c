#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "mpi.h"

#define TAG 55555


void schronisko(int ile_klientow)
{
    std::cout << "Wez psa za darmo!" << std::endl;

    int ilosc_psow = (rand()%ile_klientow) + 1;
    while(ilosc_psow > 0 && ile_klientow > 0)
    {
        int dzialanie_klient;
        MPI_Status status;
        MPI_Recv(&dzialanie_klient,1,MPI_INT,MPI_ANY_SOURCE,TAG,MPI_COMM_WORLD, &status);

        switch (dzialanie_klient)
        {
        case 1:
            std::cout << "Klient bierze psa" << std::endl;
            ile_klientow--;
            if(ilosc_psow > 1)
                std::cout << "Zostalo: " << --ilosc_psow << " psow." << std::endl;
            else
                std::cout << "To byl ostatni pies" << std::endl;
            break;

        case 2:
            std::cout << "Klient sie zastanawia" << std::endl;
            std::cout << "Nikt nie wzial psa" << std::endl;
            break;

        case 3:
            std::cout << "Przyniesiono nowego psa do schroniska" << std::endl;
            std::cout << "Stan schroniska: " << ++ilosc_psow << " psow." << std::endl;
            break;

        case 4:
            std::cout << "Dowieziono karme dla psow" << std::endl;
            break;

        case 5:
            std::cout << "Przybyl wolontariat. Bawia sie z psami i sa szczesliwe" << std::endl;
            break;

        case 6:
            ile_klientow--;
            if(ilosc_psow >= 2)
            {
                std::cout << "Klient wzial 2 psy." << std::endl;
                ilosc_psow -= 2;
                if(ilosc_psow >= 1)
                    std::cout << "Stan schroniska: " << ilosc_psow << " psow." << std::endl;
                else
                    std::cout << "Ostatnie dwa psy zostaly oddane" << std::endl;
            }
            else 
            {
                std::cout << "Klient chcial wziac dwa psy. Lecz zostal tylko jeden ktorego zabral." << std::endl;
            }
            break;

        default:
            std::cout << "Zaszla pomylka. Nie zostal oddany pies" << std::endl;
            ile_klientow--;
            break;
        }

        std::cout << "Aktualny ruch: " << ile_klientow << " klientow" << std::endl;
    }

    if (ile_klientow == 0 && ilosc_psow == 0)
        std::cout << "Ostatni klient zabral ostatniego psa." << std::endl;
    else if(ile_klientow <= 0 )
        std::cout << "Zamykamy... Ostatni klient wyszedl" << std::endl;
    else if (ilosc_psow == 0)
        std::cout << "Brak psow w schronisku. Zabrano wszystkie." << std::endl;
}

void klient(int nr_klienta)
{
    while (true)
    {
        int dzialanie_klient = rand()%7;
        switch (dzialanie_klient)
        {
        case 1:
        {
            std::cout << "Ale slodnki piesek. Klient nr: " << nr_klienta << " z checia go wezme." << std::endl;
            return;
        }

        case 2:
        {
            std::cout << "Klient nr " << nr_klienta << " nie jestem pewien czy chce tego psa." << std::endl;
            break;
        }

        case 3:
        {
            std::cout << "Klient nr " << nr_klienta << " przyniosl nowego psa." << std::endl;
            std::cout << "Szuka psa dla siebie..." << std::endl;
            break;
        }
            
        case 4:
        {
            int ilosc_karmky = 1 + (rand() % 10);
            std::cout << "Klient nr " << nr_klienta << " przyniosl " << ilosc_karmy << "kg karmy." << std::endl;
            break;
        }
            
        case 5:
        {
            std::cout << "Klient nr " << nr_klienta << " przyszedl pobawic sie z psami" << std::endl;
            break;
        }

        case 6:
        {
            std::cout << "Klient nr " << nr_klienta << " chce dwa psy." << std::endl;
            return;
        }

        default:
        {
            std::cout << "Pomylka nie tego szukano" << std::endl;
            std::cout << "Klient nr " << nr_klienta << " idzie do innego schroniska." << std::endl;
            return;
        }
        }
        MPI_Send(&dzialanie_klient, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
        int czas_czekania = 1 + (rand() % 3);
        sleep(czas_czekania);
    }
}

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int nr_procesu;
    MPI_Comm_rank(MPI_COMM_WORLD, &nr_procesu);

    int ilosc_procesow;
    MPI_Comm_size(MPI_COMM_WORLD, &ilosc_procesow);

    srand(time(NULL) ^ 56621 ^ 185213<< nr_procesu);
    if(nr_procesu == 0)
        schronisko(ilosc_procesow-1);
    else
        klient(nr_procesu);

    MPI_Finalize();
}