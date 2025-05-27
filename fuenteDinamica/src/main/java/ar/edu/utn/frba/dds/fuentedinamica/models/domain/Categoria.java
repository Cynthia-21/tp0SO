package ar.edu.utn.frba.dds.fuentedinamica.models.domain;

import lombok.Getter;

@Getter
public class Categoria {
  private String nombre;

  public Categoria(String nombre) {
    this.nombre = nombre;
  }
}
