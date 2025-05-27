package ar.edu.utn.frba.dds.fuentedinamica.models.domain;

import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.time.Period;

@Getter
@Setter
public class Contribuyente {
  private String nombre;
  private String apellido;
  private LocalDate fechaNacimiento;
  private boolean estaRegistrado;

  public Contribuyente(String nombre, String apellido, LocalDate fechaNacimiento, boolean estaRegistrado) {
    this.nombre = nombre;
    this.apellido = apellido;
    this.fechaNacimiento = fechaNacimiento;
    this.estaRegistrado = estaRegistrado;
  }

  public Integer edad() {
    return Period.between(this.fechaNacimiento, LocalDate.now()).getYears();
  }

}
