## 1. Versão Embedded
```javascript

use catalogo_embedded;

db.plataformas.insertMany([{
    nome: "PC",
    fabricante: "Vários",
    ano_lancamento: 1980,
    tipo: "Computador"},
  {
    nome: "PlayStation 4",
    fabricante: "Sony",
    ano_lancamento: 2015,
    tipo: "Console"
  },
  {
    nome: "Xbox One",
    fabricante: "Microsoft",
    ano_lancamento: 2016,
    tipo: "Console"
  },
  {
    nome: "Nintendo Switch",
    fabricante: "Nintendo",
    ano_lancamento: 2017,
    tipo: "Console Híbrido"
  },
  {
    nome: "PlayStation 5",
    fabricante: "Sony",
    ano_lancamento: 2020,
    tipo: "Console"
  }
]);

db.jogos.insertMany([
  {
    nome: "The Witcher 3",
    ano_lancamento: 2015,
    genero: "RPG",
    nota_critica: 9.8,
    desenvolvedor: {
      nome: "CD Projekt RED",
      pais: "Polonia",
      ano_fundacao: 1994,
      num_funcionarios: 800
    },
    plataformas: [
      db.plataformas.findOne({nome: "PC"}),
      db.plataformas.findOne({nome: "PlayStation 4"}),
      db.plataformas.findOne({nome: "Xbox One"}),
      db.plataformas.findOne({nome: "Nintendo Switch"})
    ]
  },
  {
    nome: "Cyberpunk 2077",
    ano_lancamento: 2020,
    genero: "RPG",
    nota_critica: 7.5,
    desenvolvedor: {
      nome: "CD Projekt RED",
      pais: "Polonia",
      ano_fundacao: 1994,
      num_funcionarios: 800
    },
    plataformas: [
      db.plataformas.findOne({nome: "PC"}),
      db.plataformas.findOne({nome: "PlayStation 4"}),
    ]
  },
  {
    nome: "Minecraft",
    ano_lancamento: 2011,
    genero: "Sandbox",
    nota_critica: 9.0,
    desenvolvedor: {
      nome: "Mojang Studios",
      pais: "Suecia",
      ano_fundacao: 2009,
      num_funcionarios: 600
    },
    plataformas: [
      db.plataformas.findOne({nome: "PC"}),
      db.plataformas.findOne({nome: "PlayStation 4"}),
      db.plataformas.findOne({nome: "Nintendo Switch"}),
    ]
  }
]);
```

### Consultas - Versão Embedded

```javascript
db.jogos.find(
  { ano_lancamento: { $gt: 2015 } },
  { 
    nome: 1,
    ano_lancamento: 1,
    genero: 1,
    "desenvolvedor.nome": 1,
    "desenvolvedor.pais": 1
  }
);

db.jogos.find(
  { 
    "plataformas.nome": "Nintendo Switch",
    $or: [
      { "desenvolvedor.pais": "Polonia" },
      { "desenvolvedor.pais": "Suecia" }
    ]
  },
  {
    nome: 1,
    nota_critica: 1,
    "desenvolvedor.nome": 1,
    "desenvolvedor.pais": 1,
    "plataformas.nome": 1
  }
);
```

---

## 2. Versão Referenced


```javascript

use catalogo_jogos_referenced;


db.desenvolvedores.insertMany([
  {
    nome: "CD Projekt RED",
    pais: "Polonia",
    ano_fundacao: 1994,
    num_funcionarios: 800
  },
  {
    nome: "Mojang Studios",
    pais: "Suecia",
    ano_fundacao: 2009,
    num_funcionarios: 600
  }
]);


db.plataformas.insertMany([
  {
    nome: "PC",
    fabricante: "Vários",
    ano_lancamento: 1980,
    tipo: "Computador"
  },
  {
    nome: "PlayStation 4",
    fabricante: "Sony",
    ano_lancamento: 2013,
    tipo: "Console"
  },
  {
    nome: "Xbox One",
    fabricante: "Microsoft",
    ano_lancamento: 2013,
    tipo: "Console"
  },
  {
    nome: "Nintendo Switch",
    fabricante: "Nintendo",
    ano_lancamento: 2017,
    tipo: "Console Híbrido"
  },
  {
    nome: "PlayStation 5",
    fabricante: "Sony",
    ano_lancamento: 2020,
    tipo: "Console"
  },
  {
    nome: "Mobile",
    fabricante: "Vários",
    ano_lancamento: 2007,
    tipo: "Dispositivo Móvel"
  }
]);


db.jogos.insertMany([
  {
    nome: "The Witcher 3",
    ano_lancamento: 2015,
    genero: "RPG",
    nota_critica: 9.8,
    desenvolvedor_id: db.desenvolvedores.findOne({nome: "CD Projekt RED"})._id,
    plataforma_ids: [
      db.plataformas.findOne({nome: "PC"})._id,
      db.plataformas.findOne({nome: "PlayStation 4"})._id,
      db.plataformas.findOne({nome: "Xbox One"})._id,
      db.plataformas.findOne({nome: "Nintendo Switch"})._id
    ]
  },
  {
    nome: "Cyberpunk 2077",
    ano_lancamento: 2020,
    genero: "RPG",
    nota_critica: 7.5,
    desenvolvedor_id: db.desenvolvedores.findOne({nome: "CD Projekt RED"})._id,
    plataforma_ids: [
      db.plataformas.findOne({nome: "PC"})._id,
      db.plataformas.findOne({nome: "PlayStation 4"})._id,
      db.plataformas.findOne({nome: "PlayStation 5"})._id,
      db.plataformas.findOne({nome: "Xbox One"})._id
    ]
  },
  {
    nome: "Minecraft",
    ano_lancamento: 2011,
    genero: "Sandbox",
    nota_critica: 9.0,
    desenvolvedor_id: db.desenvolvedores.findOne({nome: "Mojang Studios"})._id,
    plataforma_ids: [
      db.plataformas.findOne({nome: "PC"})._id,
      db.plataformas.findOne({nome: "PlayStation 4"})._id,
      db.plataformas.findOne({nome: "Xbox One"})._id,
      db.plataformas.findOne({nome: "Nintendo Switch"})._id,
      db.plataformas.findOne({nome: "Mobile"})._id
    ]
  }
]);
```

### Consultas - Versão Referenced

```javascript

db.jogos.aggregate([
  { $match: { ano_lancamento: { $gt: 2015 } } },
  { $lookup: {
      from: "desenvolvedores",
      localField: "desenvolvedor_id",
      foreignField: "_id",
      as: "desenvolvedor"
  }},
  { $unwind: "$desenvolvedor" },
  { $project: {
      "jogo": "$nome",
      "ano": "$ano_lancamento",
      "genero": "$genero",
      "desenvolvedor": "$desenvolvedor.nome",
      "pais": "$desenvolvedor.pais"
  }}
]);

db.jogos.aggregate([
  { $lookup: {
      from: "plataformas",
      localField: "plataforma_ids",
      foreignField: "_id",
      as: "plataformas"
  }},
  { $match: { "plataformas.nome": "Nintendo Switch" } },
  { $lookup: {
      from: "desenvolvedores",
      localField: "desenvolvedor_id",
      foreignField: "_id",
      as: "desenvolvedor"
  }},
  { $unwind: "$desenvolvedor" },
  { $project: {
      "jogo": "$nome",
      "desenvolvedor": "$desenvolvedor.nome",
      "nota": "$nota_critica",
      "plataformas": "$plataformas.nome"
  }}
]);
```
