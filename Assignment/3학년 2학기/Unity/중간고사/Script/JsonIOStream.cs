using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO;
using System;
using UnityEngine.UIElements;
using System.Text;

public static class JsonHelper
{
    class Wrapper<T>
    {
        public T[] items;
    }

    public static T[] FromJson<T>(string json)
    {        
        Wrapper<T> wrapper = JsonUtility.FromJson<Wrapper<T>>(json);
        return wrapper.items;
    }

    public static string ToJson<T>(T[] array)
    {
        Wrapper<T> wrapper = new Wrapper<T>();
        wrapper.items = array;
        return JsonUtility.ToJson(wrapper);
    }
}

public class JsonIOStream
{
    public static T[] Json_Load<T>(string path)
    {
        FileStream file = new FileStream(Application.dataPath + "/" + path + ".json", FileMode.Open);
        if (!File.Exists(Application.dataPath + "/" + path + ".json"))
            return null;

        byte[] data = new byte[file.Length];
        file.Read(data, 0, data.Length);
        file.Close();

        string json = Encoding.UTF8.GetString(data);
        return JsonHelper.FromJson<T>(json);
    }

    public static void Json_Save(string jsondata, string path)
    {
        FileStream file = new FileStream(Application.dataPath + "/" + path + ".json", FileMode.Open);
        byte[] data = Encoding.UTF8.GetBytes(jsondata);
        file.Write(data, 0, data.Length);
        file.Close();
    }
}
