package ar.edu.utn.frba.dds.servicioagregador.models.domain;

import lombok.Getter;

@Getter
public class Administrador {
  private String nombre;
  private String apellido;

  public Administrador(String nombre, String apellido) {
    this.nombre = nombre;
    this.apellido = apellido;
  }

  public Administrador(String nombre) {
    this.nombre = nombre;
  }
}
