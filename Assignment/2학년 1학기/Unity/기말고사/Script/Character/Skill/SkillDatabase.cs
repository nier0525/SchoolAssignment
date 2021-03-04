using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SkillDatabase : MonoBehaviour
{
    [HideInInspector] public int Meteo;
    [HideInInspector] public int M_fire;
    [HideInInspector] public int Blizzard;
    [HideInInspector] public int Icestorm;
    [HideInInspector] public int Iceorb;
    [HideInInspector] public int Icebolt;
    [HideInInspector] public int Blaze;
    [HideInInspector] public int Firewall;
    [HideInInspector] public int Firebolt;
    [HideInInspector] public int Fireball;

    [HideInInspector] public int Blizzard_mp;
    [HideInInspector] public int Iceorb_mp;
    [HideInInspector] public int Blaze_mp;
    [HideInInspector] public int Firewall_mp;
    [HideInInspector] public int Firebolt_mp;
    [HideInInspector] public int Fireball_mp;
    [HideInInspector] public int Meteo_mp;

    private void Awake()
    {
        Blizzard_mp = 100;
        Iceorb_mp = 50;
        Blaze_mp = 50;
        Firewall_mp = 100;
        Firebolt_mp = 5;
        Fireball_mp = 10;
        Meteo_mp = 150;
    }

    private void Update()
    {
        Blizzard = blizzard.damage;
        Icestorm = icestorm.damage;
        Iceorb = iceorb.damage;
        Icebolt = iceboltController.damage;
        Blaze = hydragroundfire.damage;
        Firewall = firewall.damage;
        Firebolt = firebolt.damage;
        Fireball = fireball.damage;
        M_fire = m_fire.damage;
        Meteo = meteor.damage;
    }
}