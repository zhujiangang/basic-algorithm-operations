package com.e2u.alg.montyhall;
import java.util.Random;

public class MontyHall {
    public static final Random gen = new Random();
    public static final int ROUNDS = 10000;
 
    /** chooses a random door other than door1 or door2 */
    private static int chooseAnotherDoor(int door1, int door2) {
        int result;
        do
            result = gen.nextInt(3);
        while (result == door1 || result == door2);
        return result;
    	
//    	return 3 - (door1 + door2);
    }
 
    public static void main(String[] args) {
        System.out.println("begin playing " + ROUNDS + " rounds");
 
        int wins = 0;
        for (int i = 0; i < ROUNDS; i++) {
            int prize = gen.nextInt(3);
            int userChoice1 = gen.nextInt(3);
            // host opens door other than user's choice without prize
            int hostChoice = chooseAnotherDoor(prize, userChoice1);
            // user always switches
            int userChoice2 = chooseAnotherDoor(userChoice1, hostChoice);
            if (userChoice2 == prize)
                wins++;
        }
 
        System.out.println(wins + " wins by switching");
    }
}